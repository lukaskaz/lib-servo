#pragma once

#include "log/interfaces/logs.hpp"
#include "servo/factory.hpp"

#include <tuple>
#include <vector>

namespace servo::group
{

using config_t = std::tuple<std::vector<std::shared_ptr<servo::ServoIf>>,
                            std::shared_ptr<logs::LogIf>>;

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

} // namespace servo::group
