#include "servo/interfaces/rpi/pca9685/servo.hpp"

#include "pwm/interfaces/rpi/pca9685/pwm.hpp"

#include <functional>
#include <future>
#include <source_location>

namespace servo::rpi::pca9685
{

struct Servo::Handler
{
  public:
    explicit Handler(const config_t& config) :
        logif{std::get<5>(config)},
        driverpath{std::get<0>(config)}, id{std::get<1>(config)},
        startpos{std::get<3>(config)}, endpos{std::get<4>(config)}
    {
        using namespace pwm::rpi::pca9685;
        pwmif = pwm::Factory::create<Pwm, pwm::rpi::pca9685::config_t>(
            {id, 0, freqhz, polaritytype::normal, "/sys/class/pwm/pwmchip2",
             logif});

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
                            std::to_string(duty) + "' from perc '" +
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
                            std::to_string(duty) + "' from perc '" +
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
        log(logging::type::info, "Removed servo: " + std::to_string(id));
    }

    bool movestart()
    {
        return setpwm(pctmin);
    }

    bool moveend()
    {
        return setpwm(pctmax);
    }

    bool moveto(double pctpos)
    {
        return setpwm(pctpos);
    }

  private:
    const std::shared_ptr<logging::LogIf> logif;
    const std::string driverpath;
    const uint32_t id;
    const double pctmin{0.};
    const double pctmax{100.};
    const double startpos;
    const double endpos;
    const uint32_t freqhz{50}; // -> const uint32_t period{20000000};
    std::function<double(double)> dutycalc;
    std::future<void> async;
    std::shared_ptr<pwm::PwmIf> pwmif;

    bool setpwm(double pctpos)
    {
        if (pctpos >= pctmin && pctpos <= pctmax)
            return useasync(
                [this, pctpos]() { pwmif->setduty(dutycalc(pctpos)); });
        throw std::runtime_error(
            "Servo set to move outside range of percent position: " +
            std::to_string(pctpos));
    }

    bool useasync(std::function<void()>&& func)
    {
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
