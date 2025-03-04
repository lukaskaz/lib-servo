#include "logs/interfaces/console/logs.hpp"
#include "servo/interfaces/digital/rpi/pca9685/servo.hpp"
#include "servo/interfaces/group/servo.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <vector>

int main(int argc, char** argv)
{
    try
    {
        if (argc >= 4)
        {
            std::cout
                << "First scenario -> moving all servos to given position\n";
            const auto amount = (int32_t)atoi(argv[1]);
            const auto pospct = (double)atoi(argv[2]);
            const auto loglvl =
                (bool)atoi(argv[3]) ? logs::level::debug : logs::level::info;
            const auto driverpath =
                argc < 5 ? "/sys/class/pwm/pwmchip2/" : argv[4];
            auto sequence = std::views::iota(0, amount);
            std::vector<std::shared_ptr<servo::ServoIf>> servos;

            auto logif = logs::Factory::create<logs::console::Log,
                                               logs::console::config_t>(loglvl);
            std::ranges::for_each(
                sequence, [&driverpath, &servos, logif](uint8_t num) {
                    using namespace servo::rpi::pca9685;
                    // use spec to provide parameters for api, i.e. Savox
                    // SH-0254MG: refreshrate: 240hz; neutral position: 1500us;
                    // max travel 800us -> 2200us [150dgr];
                    static const auto refreshratehz{240};
                    static const auto neutralpos{1500us}, leftpos{2200us},
                        rightpos{800us};
                    auto iface = servo::Factory::create<Servo, config_t>(
                        {driverpath, num,
                         num % 2 == 0 ? mounttype::normal : mounttype::inverted,
                         refreshratehz, neutralpos, leftpos, rightpos, logif});
                    servos.push_back(iface);
                });
            auto group =
                servo::Factory::create<servo::group::Servo,
                                       servo::group::config_t>({servos, logif});

            group->moveto(pospct);
            std::cout << "Moving to servo position: "
                      << std::quoted(std::to_string(pospct))
                      << "[%]\nPress [enter]" << std::flush;
            getchar();

            group->movecenter();
            std::cout << "Moving to servo position: " << std::quoted("NEUTRAL")
                      << "\nPress [enter]" << std::flush;
            getchar();

            group->moveright();
            std::cout << "Moving to servo position: " << std::quoted("RIGHT")
                      << "\nPress [enter]" << std::flush;
            getchar();

            group->moveleft();
            std::cout << "Moving to servo position: " << std::quoted("LEFT")
                      << "\nPress [enter]" << std::flush;
            getchar();

            std::cout << "First scenario DONE -> disengaging servos\n";
        }
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }
    return 0;
}
