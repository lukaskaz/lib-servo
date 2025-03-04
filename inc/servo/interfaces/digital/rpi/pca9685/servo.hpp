#pragma once

#include "log/interfaces/logs.hpp"
#include "servo/factory.hpp"

#include <chrono>
#include <cstdint>
#include <tuple>

using namespace std::chrono_literals;

namespace servo::rpi::pca9685
{

enum class mounttype
{
    normal,
    inverted
};

using config_t =
    std::tuple<std::string, uint32_t, mounttype, uint32_t,
               std::chrono::microseconds, std::chrono::microseconds,
               std::chrono::microseconds, std::shared_ptr<logs::LogIf>>;

class Servo : public ServoIf
{
  public:
    ~Servo();
    bool moveleft() override;
    bool moveright() override;
    bool movecenter() override;
    bool moveto(double) override;

  private:
    friend class servo::Factory;
    explicit Servo(const config_t&);

    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace servo::rpi::pca9685
