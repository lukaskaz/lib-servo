#include "servo/interfaces/group/servo.hpp"

#include <algorithm>
#include <functional>
#include <future>
#include <ranges>
#include <source_location>

namespace servo::group
{

struct Servo::Handler
{
  public:
    explicit Handler(const config_t& config) :
        logif{std::get<1>(config)}, servos{std::get<0>(config)}
    {
        log(logs::level::info,
            "Created group of servos num: " + std::to_string(servos.size()));
    }

    ~Handler()
    {
        log(logs::level::info,
            "Removed group of servos num: " + std::to_string(servos.size()));
    }

    bool moveleft()
    {
        std::ranges::for_each(servos,
                              [this](const auto servo) { servo->moveleft(); });
        return true;
    }

    bool moveright()
    {
        std::ranges::for_each(servos,
                              [this](const auto servo) { servo->moveright(); });
        return true;
    }

    bool movecenter()
    {
        std::ranges::for_each(
            servos, [this](const auto servo) { servo->movecenter(); });
        return true;
    }

    bool moveto(double pospct)
    {
        std::ranges::for_each(servos, [this, pospct](const auto servo) {
            servo->moveto(pospct);
        });
        return true;
    }

  private:
    const std::shared_ptr<logs::LogIf> logif;
    const std::vector<std::shared_ptr<servo::ServoIf>> servos;
    std::future<void> async;

    bool useasync(std::function<void()>&& func)
    {
        return true;
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

} // namespace servo::group
