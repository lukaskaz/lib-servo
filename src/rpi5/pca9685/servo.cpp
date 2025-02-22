#include "servo/interfaces/rpi5/pca9685/servo.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>

namespace servo::rpi5::pca9685
{

using namespace std::chrono_literals;

struct Servo::Handler
{
  public:
    explicit Handler(const config_t& config)
    {
        std::ranges::for_each(config, [this](const auto& servoconfig) {
            group.push_back(std::make_shared<Single>(servoconfig));
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
        explicit Single(const servoconfig_t& config) :
            id{num++}, startpos{std::get<1>(config)},
            endpos{std::get<2>(config)}
        {
            writesysfs("/sys/class/pwm/pwmchip0/export", id);
            writesysfs("/sys/class/pwm/pwmchip0/pwm" + std::to_string(id) +
                           "/period",
                       pwmperiod);

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
            writesysfs("/sys/class/pwm/pwmchip0/unexport", id);
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
        const uint32_t id;
        const int32_t pctmin{0};
        const int32_t pctmax{100};
        const int32_t startpos;
        const int32_t endpos;
        const uint32_t pwmperiod{20000000};
        std::function<int32_t(int32_t)> dutycalc;
        static uint32_t num;

        bool writesysfs(const std::string& path, uint32_t val,
                        std::chrono::milliseconds delay = 25ms) const
        {
            std::ofstream ofs(path);
            if (!ofs.is_open())
            {
                throw std::runtime_error("Cannot open sysfs file " + path);
            }
            ofs << val;
            ofs.close();
            usleep((uint32_t)delay.count() * 1000);
            return true;
        }

        bool setpwm(uint32_t val) const
        {
            std::cout << "setting duty -> " << val << std::endl;
            return writesysfs("/sys/class/pwm/pwmchip0/pwm" +
                                  std::to_string(id) + "/duty_cycle",
                              val);
        }
    };
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

} // namespace servo::rpi5::pca9685