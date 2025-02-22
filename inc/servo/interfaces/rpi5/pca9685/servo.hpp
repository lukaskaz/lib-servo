#pragma once

#include "servo/factory.hpp"

#include <tuple>

namespace servo::rpi5::pca9685
{

enum class mounttype
{
    normal,
    inverted
};

using servoconfig_t = std::tuple<mounttype, int32_t, int32_t>;
using config_t = std::vector<servoconfig_t>;

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

} // namespace servo::rpi5::pca9685
