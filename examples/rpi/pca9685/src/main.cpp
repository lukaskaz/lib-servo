#include "log/interfaces/console.hpp"
#include "servo/interfaces/group/servo.hpp"
#include "servo/interfaces/rpi/pca9685/servo.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

int main(int argc, char** argv)
{
    try
    {
        if (argc == 4)
        {
            std::cout
                << "First scenario -> moving all servos to given position\n";
            auto amount = (uint8_t)atoi(argv[1]);
            auto pospct = (uint8_t)atoi(argv[2]);
            auto loglvl = (bool)atoi(argv[3]) ? logging::type::debug
                                              : logging::type::info;
            // auto startpos{800000}, endpos{2000000};

            const std::string driverpath{"/sys/class/pwm/pwmchip2/"};
            auto sequence = std::views::iota(0, (int32_t)amount);
            std::vector<std::shared_ptr<servo::ServoIf>> servos;
            auto logif =
                logging::LogFactory::create<logging::console::Log>(loglvl);
            std::ranges::for_each(
                sequence, [&driverpath, &servos, logif](uint8_t num) {
                    using namespace servo::rpi::pca9685;
                    auto startpos{4}, endpos{10};
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
            std::cout << "Press [enter]" << std::flush;
            getchar();

            group->movestart();
            std::cout << "Press [enter]" << std::flush;
            getchar();

            std::cout << "First scenario DONE -> disengaging servos\n";
        }
        if (argc == 4)
        {
            // std::cout << "Second scenario -> moving single servo to given "
            //              "position\n";
            // using namespace servo::rpi::pca9685;
            // auto amount = (uint8_t)atoi(argv[1]);
            // auto pospct = (uint8_t)atoi(argv[2]);
            // auto servonum = (uint8_t)atoi(argv[3]);
            // auto startpos{800000}, endpos{2000000};
            // auto sequence = std::views::iota(0, (int32_t)amount);
            // config_t config{driverpath, {}};

            // std::ranges::for_each(sequence, [&](uint8_t num) {
            //     std::get<std::vector<servoconfig_t>>(config).emplace_back(
            //         num % 2 == 0 ? mounttype::normal : mounttype::inverted,
            //         startpos, endpos);
            // });

            // auto iface = servo::Factory::create<Servo, config_t>(config);
            // iface->moveto(servonum, pospct);
            // std::cout << "Press [enter]" << std::flush;
            // getchar();
            // iface->movestart(servonum);
            // std::cout << "Press [enter]" << std::flush;
            // getchar();
            // std::cout << "Second scenario DONE -> disengaging servo\n";
        }
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }
    return 0;
}
