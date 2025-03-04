#include "log/interfaces/console/logs.hpp"
#include "servo/interfaces/group/servo.hpp"
#include "servo/interfaces/rpi/pca9685/servo.hpp"

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
                    // const auto startpos{800000}, endpos{2000000}; <- period
                    static const auto startpos{4u}, endpos{10u};
                    auto iface = servo::Factory::create<Servo, config_t>(
                        {driverpath, num,
                         num % 2 == 0 ? mounttype::normal : mounttype::inverted,
                         startpos, endpos, logif});
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

            group->movestart();
            std::cout << "Moving to servo position: " << std::quoted("START")
                      << "\nPress [enter]" << std::flush;
            getchar();

            group->moveend();
            std::cout << "Moving to servo position: " << std::quoted("END")
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
