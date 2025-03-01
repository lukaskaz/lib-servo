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
        log(logging::type::info,
            "Created group of servos num: " + std::to_string(servos.size()));
    }

    ~Handler()
    {
        log(logging::type::info,
            "Removed group of servos num: " + std::to_string(servos.size()));
    }

    bool movestart()
    {
        std::ranges::for_each(servos,
                              [this](const auto servo) { servo->movestart(); });
        return true;
    }

    bool moveend()
    {
        std::ranges::for_each(servos,
                              [this](const auto servo) { servo->moveend(); });
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
    const std::shared_ptr<logging::LogIf> logif;
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

} // namespace servo::group
