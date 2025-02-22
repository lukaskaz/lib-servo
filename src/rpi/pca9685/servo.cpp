#include "servo/interfaces/rpi/pca9685/servo.hpp"

#include <algorithm>
#include <fstream>
#include <functional>
#include <ranges>

namespace servo::rpi::pca9685
{

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
            initdelay{std::get<1>(config)}, wrdelay{std::get<2>(config)},
            startpos{std::get<3>(config)}, endpos{std::get<4>(config)}
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

        bool moveto(int32_t pos) const
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
        const std::chrono::milliseconds initdelay;
        const std::chrono::milliseconds wrdelay;
        const int32_t startpos;
        const int32_t endpos;
        const uint32_t period{20000000};
        std::function<int32_t(int32_t)> dutycalc;

        bool writesysfs(const std::string& path, uint32_t val) const
        {
            return writesysfs(path, val, wrdelay);
        }

        bool writesysfs(const std::string& path, uint32_t val,
                        std::chrono::milliseconds delay) const
        {
            std::ofstream ofs{path};
            if (!ofs.is_open())
                throw std::runtime_error("Cannot open sysfs file " + path);
            ofs << val << std::flush;
            usleep((uint32_t)delay.count() * 1000);
            return true;
        }

        bool setup() const
        {
            writesysfs(handler->driverpath + "export", id, initdelay);
            return writesysfs(handler->driverpath + pwm + "/period", period);
        }

        bool release() const
        {
            return writesysfs(handler->driverpath + "unexport", id);
        }

        bool setpwm(uint32_t val) const
        {
            return writesysfs(handler->driverpath + pwm + "/duty_cycle", val);
        }
    };
    const std::string driverpath;
    std::vector<std::shared_ptr<Single>> group;

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
