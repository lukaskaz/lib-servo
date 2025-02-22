#pragma once

#include "servo/factory.hpp"

#include <chrono>
#include <tuple>

using namespace std::chrono_literals;

namespace servo::rpi::pca9685
{

enum class mounttype
{
    normal,
    inverted
};

using servoconfig_t = std::tuple<mounttype, std::chrono::milliseconds,
                                 std::chrono::milliseconds, int32_t, int32_t>;
using config_t = std::tuple<std::string, std::vector<servoconfig_t>>;

class Servo : public ServoIf
{
  public:
    ~Servo();
    bool movestart() override;
    bool moveend() override;
    bool moveto(int32_t) override;

    bool movestart(uint32_t) override;
    bool moveend(uint32_t) override;
    bool moveto(uint32_t, int32_t) override;

  private:
    friend class servo::Factory;
    explicit Servo(const config_t&);

    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace servo::rpi::pca9685
