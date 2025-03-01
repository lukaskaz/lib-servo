#include "servo/interfaces/rpi/pca9685/servo.hpp"

#include "pwm/interfaces/rpi/pca9685/pwm.hpp"

#include <functional>
#include <future>
#include <source_location>

namespace servo::rpi::pca9685
{

using namespace pwm::rpi::pca9685;
using namespace std::chrono_literals;
using namespace std::string_literals;

struct Servo::Handler
{
  public:
    explicit Handler(const config_t& config) :
        logif{std::get<5>(config)},
        driverpath{std::get<0>(config)}, id{std::get<1>(config)},
        startpos{std::get<3>(config)}, endpos{std::get<4>(config)}
    {
        pwmif = pwm::Factory::create<Pwm, pwm::rpi::pca9685::config_t>(
            {id, 0, freqhz, polaritytype::normal, "/sys/class/pwm/pwmchip2",
             logif});
        // setup();
        switch (std::get<2>(config))
        {
            case mounttype::normal:
                dutycalc = [this](double pctset) {
                    double duty{};
                    if (pctset == pctmin)
                        duty = startpos;
                    else if (pctset == pctmax)
                        duty = endpos;
                    else
                        duty = pctset * (endpos - startpos) / pctmax + startpos;
                    log(logging::type::debug,
                        "Servo duty[" + std::to_string(id) + "/norm]: '" +
                            std::to_string(duty) + "' from prec '" +
                            std::to_string(pctset) + "'");
                    return duty;
                };
                break;
            case mounttype::inverted:
                dutycalc = [this](double pctset) {
                    double duty{};
                    if (pctset == pctmin)
                        duty = endpos;
                    else if (pctset == pctmax)
                        duty = startpos;
                    else
                        duty = pctset * (startpos - endpos) / pctmax + endpos;
                    log(logging::type::debug,
                        "Servo duty[" + std::to_string(id) + "/invr]: '" +
                            std::to_string(duty) + "' from prec '" +
                            std::to_string(pctset) + "'");
                    return duty;
                };
                break;
        }

        log(logging::type::info,
            "Created servo [id/mount/startpos/endpos]: " + std::to_string(id) +
                "/" +
                (std::get<2>(config) == mounttype::normal ? "normal"
                                                          : "inverted") +
                "/" + std::to_string(startpos) + "/" + std::to_string(endpos));
    }

    ~Handler()
    {
        // release();
        moveend();
        // wait for servo to reach position before releasing driver
        setdelay(100ms);
        log(logging::type::info, "Removed servo: " + std::to_string(id));
    }

    bool movestart()
    {
        return pwmif->setduty(dutycalc(pctmin));
    }

    bool moveend()
    {
        return pwmif->setduty(dutycalc(pctmax));
    }

    bool moveto(double pos)
    {
        if (pos >= pctmin && pos <= pctmax)
        {
            return pwmif->setduty(dutycalc(pos));
        }
        throw std::runtime_error(
            "Servo set to move outside range of percent position");
    }

  private:
    const std::shared_ptr<logging::LogIf> logif;
    const std::string driverpath;
    const uint32_t id;
    const double pctmin{0.};
    const double pctmax{100.};
    const double startpos;
    const double endpos;
    // const uint32_t period{20000000};
    const uint32_t freqhz{50};
    std::function<double(double)> dutycalc;
    std::future<void> async;
    std::shared_ptr<pwm::PwmIf> pwmif;

    bool setup()
    {
        return useasync([this]() {
            // wrsysfscreate(handler->driverpath + "export", id,
            //               pwm + "/enable");
            // wrsysfsset(handler->driverpath + pwm + "/enable", 0);
            // wrsysfsset(handler->driverpath + pwm + "/period", period);
            // wrsysfsset(handler->driverpath + pwm + "/polarity",
            // "normal"s);
            // // wrsysfsset(handler->driverpath + pwm + "/polarity",
            // //            "inversed"s);
            // wrsysfsset(handler->driverpath + pwm + "/enable", 1);
        });
    }

    bool release()
    {
        return useasync([this]() {
            // wrsysfsset(handler->driverpath + pwm + "/enable", 0);
            // wrsysfsremove(handler->driverpath + "unexport", id, pwm);
        });
    }

    bool setpwm(uint32_t val)
    {
        return useasync([this, val]() {
            // wrsysfsset(handler->driverpath + pwm + "/duty_cycle", val);
        });
    }

    bool useasync(std::function<void()>&& func)
    {
        return true;
        if (async.valid())
            async.wait();
        async = std::async(std::launch::async, std::move(func));
        return true;
    };

    void log(
        logging::type type, const std::string& msg,
        const std::source_location loc = std::source_location::current()) const
    {
        if (logif)
            logif->log(type, std::string{loc.function_name()}, msg);
    }

    void setdelay(std::chrono::milliseconds time) const
    {
        usleep((uint32_t)time.count() * 1000);
    }

    // std::shared_ptr<Single> getservo(uint32_t num)
    // {
    //     throw std::runtime_error(
    //         "Requested servo number is outside defined group");
    // }
};

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

bool Servo::moveto(double pos)
{
    return handler->moveto(pos);
}

} // namespace servo::rpi::pca9685
