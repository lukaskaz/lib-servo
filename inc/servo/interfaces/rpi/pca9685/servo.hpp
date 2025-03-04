#pragma once

#include "logs/interfaces/logs.hpp"
#include "servo/factory.hpp"

#include <cstdint>
#include <tuple>

namespace servo::rpi::pca9685
{

enum class mounttype
{
    normal,
    inverted
};

using config_t = std::tuple<std::string, uint32_t, mounttype, double, double,
                            std::shared_ptr<logs::LogIf>>;

class Servo : public ServoIf
{
  public:
    ~Servo();
    bool movestart() override;
    bool moveend() override;
    bool moveto(double) override;

  private:
    friend class servo::Factory;
    explicit Servo(const config_t&);

    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace servo::rpi::pca9685
