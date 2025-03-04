#include "servo/interfaces/digital/rpi/pca9685/servo.hpp"

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
        logif{std::get<7>(config)}, id{std::get<1>(config)},
        refreshrate{std::get<3>(config)}, posneutral{posfrompulse(
                                              std::get<4>(config))},
        posleft{posfrompulse(std::get<5>(config))}, posright{posfrompulse(
                                                        std::get<6>(config))}
    {
        auto driver{std::get<0>(config)};
        auto mount{std::get<2>(config)};

        using namespace pwm::rpi::pca9685;
        pwmif = pwm::Factory::create<Pwm, pwm::rpi::pca9685::config_t>(
            {id, 0, refreshrate, polaritytype::normal, driver, logif});

        switch (mount)
        {
            case mounttype::normal:
                dutycalc = [this](double pctset) {
                    double duty{};
                    if (pctset == pctmin)
                        duty = posleft;
                    else if (pctset == pctmax)
                        duty = posright;
                    else if (pctset == pctmid)
                        duty = posneutral;
                    else
                        duty = (pctset - pctmin) * (posright - posleft) /
                                   (pctmax - pctmin) +
                               posleft;
                    log(logs::level::debug,
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
                        duty = posright;
                    else if (pctset == pctmax)
                        duty = posleft;
                    else if (pctset == pctmid)
                        duty = posneutral;
                    else
                        duty = (pctset - pctmin) * (posleft - posright) /
                                   (pctmax - pctmin) +
                               posright;
                    log(logs::level::debug,
                        "Servo duty[" + std::to_string(id) + "/invr]: '" +
                            std::to_string(duty) + "' from perc '" +
                            std::to_string(pctset) + "'");
                    return duty;
                };
                break;
        }

        log(logs::level::info,
            "Created servo [id/mount/startpos/endpos]: " + std::to_string(id) +
                "/" + (mount == mounttype::normal ? "normal" : "inverted") +
                "/" + std::to_string(posleft) + "/" + std::to_string(posright));
    }

    ~Handler()
    {
        log(logs::level::info, "Removed servo: " + std::to_string(id));
    }

    bool moveleft()
    {
        return setpwm(pctmin);
    }

    bool moveright()
    {
        return setpwm(pctmax);
    }

    bool movecenter()
    {
        return setpwm(pctmid);
    }

    bool moveto(double pctpos)
    {
        return setpwm(pctpos);
    }

  private:
    const std::shared_ptr<logs::LogIf> logif;
    const uint32_t id;
    const double pctmin{-50.};
    const double pctmax{50.};
    const double pctmid{0.};
    // digital servos req refresh with freq 50-400 hz, let the user select
    const uint32_t refreshrate;
    const double posneutral;
    const double posleft;
    const double posright;
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

    double posfrompulse(std::chrono::microseconds pulsetime)
    {
        static constexpr auto timehz = std::chrono::microseconds{1s};
        auto period = (double)timehz.count() / refreshrate;
        auto pos = 100. * (double)pulsetime.count() / period;
        log(logs::level::debug, "Position[" + std::to_string(id) + "]: '" +
                                    std::to_string(pos) + "' from pulse '" +
                                    std::to_string(pulsetime.count()) + "'");
        return pos;
    }

    bool useasync(std::function<void()>&& func)
    {
        if (async.valid())
            async.wait();
        async = std::async(std::launch::async, std::move(func));
        return true;
    };

    void log(
        logs::level level, const std::string& msg,
        const std::source_location loc = std::source_location::current()) const
    {
        if (logif)
            logif->log(level, std::string{loc.function_name()}, msg);
    }
};

Servo::Servo(const config_t& config) :
    handler{std::make_unique<Handler>(config)}
{}
Servo::~Servo() = default;

bool Servo::moveleft()
{
    return handler->moveleft();
}

bool Servo::moveright()
{
    return handler->moveright();
}

bool Servo::movecenter()
{
    return handler->movecenter();
}

bool Servo::moveto(double pos)
{
    return handler->moveto(pos);
}

} // namespace servo::rpi::pca9685
