#include "servo/interfaces/rpi/pca9685/servo.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <ranges>

namespace servo::rpi::pca9685
{

using namespace std::chrono_literals;

struct Servo::Handler
{
  public:
    explicit Handler(const config_t& config) :
        driverpath{std::get<std::string>(config)}
    {
        std::ranges::for_each(
            std::get<std::vector<servoconfig_t>>(config),
            [this](const auto& servoconfig) {
                group.push_back(std::make_shared<Single>(this, servoconfig));
            });
    }

    ~Handler()
    {
        moveend();
        // wait for servo to reach position before releasing driver
        setdelay(100ms);
    }

    bool movestart()
    {
        std::ranges::for_each(group, [](auto servo) { servo->movestart(); });
        return true;
    }

    bool moveend()
    {
        std::ranges::for_each(group, [](auto servo) { servo->moveend(); });
        return true;
    }

    bool moveto(int32_t pos)
    {
        std::ranges::for_each(group, [pos](auto servo) { servo->moveto(pos); });
        return true;
    }

    bool movestart(uint32_t num)
    {
        return getservo(num)->movestart();
    }

    bool moveend(uint32_t num)
    {
        return getservo(num)->moveend();
    }

    bool moveto(uint32_t num, int32_t pos)
    {
        return getservo(num)->moveto(pos);
    }

  private:
    class Single
    {
      public:
        Single(Handler* handler, const servoconfig_t& config) :
            handler{handler}, id{num++}, pwm{"pwm" + std::to_string(id)},
            startpos{std::get<1>(config)}, endpos{std::get<2>(config)}
        {
            setup();
            switch (std::get<0>(config))
            {
                case mounttype::normal:
                    dutycalc = [this](int32_t pctset) {
                        if (pctset == pctmin)
                            return startpos;
                        if (pctset == pctmax)
                            return endpos;
                        return (int32_t)((endpos - startpos) / pctmax * pctset +
                                         startpos);
                    };
                    break;
                case mounttype::inverted:
                    dutycalc = [this](int32_t pctset) {
                        if (pctset == pctmin)
                            return endpos;
                        if (pctset == pctmax)
                            return startpos;
                        return (int32_t)((startpos - endpos) / pctmax * pctset +
                                         endpos);
                    };
                    break;
            }
        }
        ~Single()
        {
            release();
        }

        bool movestart()
        {
            return setpwm(dutycalc(pctmin));
        }

        bool moveend()
        {
            return setpwm(dutycalc(pctmax));
        }

        bool moveto(int32_t pos)
        {
            if (pos >= pctmin && pos <= pctmax)
            {
                return setpwm(dutycalc(pos));
            }
            throw std::runtime_error(
                "Servo set to move outside range of percent position");
        }

      private:
        static uint32_t num;
        const Handler* const handler;
        const uint32_t id;
        const std::string pwm;
        const int32_t pctmin{0};
        const int32_t pctmax{100};
        const int32_t startpos;
        const int32_t endpos;
        const uint32_t period{20000000};
        std::function<int32_t(int32_t)> dutycalc;
        std::future<void> async;

        bool wrsysfs(const std::string& path, uint32_t val) const
        {
            if (std::ofstream ofs{path}; ofs.is_open())
            {
                ofs << val << std::flush;
                return true;
            }
            throw std::runtime_error("Cannot open sysfs output file " + path);
        }

        bool rdsysfs(const std::string& path, uint32_t& ret) const
        {
            if (std::ifstream ifs{path}; ifs.is_open())
            {
                ifs >> ret;
                return true;
            }
            throw std::runtime_error("Cannot open sysfs input file " + path);
        }

        bool wrsysfsset(const std::string& path, const uint32_t val) const
        {
            if (uint32_t ret{};
                wrsysfs(path, val) && rdsysfs(path, ret) && ret == val)
                return true;
            throw std::runtime_error("Cannot set value for sysfs file " + path);
        }

        bool wrsysfscreate(const std::string& path, uint32_t val,
                           const std::string& name) const
        {
            const auto directory =
                std::filesystem::path{path}.parent_path() / name;
            if (wrsysfs(path, val))
            {
                uint32_t retries{100};
                while (retries--)
                {
                    handler->setdelay(1ms);
                    if (std::filesystem::exists(directory) &&
                        std::ofstream{directory}.is_open())
                        return true;
                }
            }
            throw std::runtime_error("Cannot create module via sysfs " +
                                     std::string(directory));
        }
        bool wrsysfsremove(const std::string& path, uint32_t val,
                           const std::string& name) const
        {
            const auto directory =
                std::filesystem::path{path}.parent_path() / name;
            if (wrsysfs(path, val))
            {
                uint32_t retries{100};
                while (retries--)
                {
                    if (!std::filesystem::exists(directory))
                        return true;
                    handler->setdelay(1ms);
                }
            }
            throw std::runtime_error("Cannot remove module via sysfs " +
                                     std::string(directory));
        }

        bool setup()
        {
            return useasync([this]() {
                wrsysfscreate(handler->driverpath + "export", id,
                              pwm + "/period");
                wrsysfsset(handler->driverpath + pwm + "/period", period);
            });
        }

        bool release()
        {
            return useasync([this]() {
                wrsysfsremove(handler->driverpath + "unexport", id, pwm);
            });
        }

        bool setpwm(uint32_t val)
        {
            return useasync([this, val]() {
                wrsysfsset(handler->driverpath + pwm + "/duty_cycle", val);
            });
        }

        bool useasync(std::function<void()>&& func)
        {
            if (async.valid())
                async.wait();
            async = std::async(std::launch::async, std::move(func));
            return true;
        };
    };
    const std::string driverpath;
    std::vector<std::shared_ptr<Single>> group;

    void setdelay(std::chrono::milliseconds time) const
    {
        usleep((uint32_t)time.count() * 1000);
    }

    std::shared_ptr<Single> getservo(uint32_t num)
    {
        if (num < group.size())
            return group[num];
        throw std::runtime_error(
            "Requested servo number is outside defined group");
    }
};
uint32_t Servo::Handler::Single::num{0};

Servo::Servo(const config_t& config) :
    handler{std::make_unique<Handler>(config)}
{}
Servo::~Servo() = default;

bool Servo::movestart()
{
    return handler->movestart();
}

bool Servo::moveend()
{
    return handler->moveend();
}

bool Servo::moveto(int32_t pos)
{
    return handler->moveto(pos);
}

bool Servo::movestart(uint32_t num)
{
    return handler->movestart(num);
}

bool Servo::moveend(uint32_t num)
{
    return handler->moveend(num);
}

bool Servo::moveto(uint32_t num, int32_t pos)
{
    return handler->moveto(num, pos);
}

} // namespace servo::rpi::pca9685
