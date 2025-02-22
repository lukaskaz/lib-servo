#include "servo/interfaces/rpi/pca9685/servo.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>

int main(int argc, char** argv)
{
    try
    {
        const std::string driverpath{"/sys/class/pwm/pwmchip0/"};
        auto initlatency{30ms}, cmdslatency{5ms};
        if (argc == 3)
        {
            std::cout
                << "First scenario -> moving all servos to given position\n";
            using namespace servo::rpi::pca9685;
            auto amount = (uint8_t)atoi(argv[1]);
            auto pospct = (uint8_t)atoi(argv[2]);
            auto startpos{800000}, endpos{2000000};
            auto sequence = std::views::iota(0, (int32_t)amount);
            config_t config{driverpath, {}};

            std::ranges::for_each(sequence, [&](uint8_t num) {
                std::get<std::vector<servoconfig_t>>(config).emplace_back(
                    num % 2 == 0 ? mounttype::normal : mounttype::inverted,
                    initlatency, cmdslatency, startpos, endpos);
            });

            auto iface = servo::Factory::create<Servo, config_t>(config);
            iface->moveto(pospct);
            std::cout << "Press [enter]" << std::flush;
            getchar();
            iface->movestart();
            std::cout << "Press [enter]" << std::flush;
            getchar();
            std::cout << "First scenario DONE -> disengaging servos\n";
        }
        if (argc == 4)
        {
            std::cout << "Second scenario -> moving single servo to given "
                         "position\n";
            using namespace servo::rpi::pca9685;
            auto amount = (uint8_t)atoi(argv[1]);
            auto pospct = (uint8_t)atoi(argv[2]);
            auto servonum = (uint8_t)atoi(argv[3]);
            auto startpos{800000}, endpos{2000000};
            auto sequence = std::views::iota(0, (int32_t)amount);
            config_t config{driverpath, {}};

            std::ranges::for_each(sequence, [&](uint8_t num) {
                std::get<std::vector<servoconfig_t>>(config).emplace_back(
                    num % 2 == 0 ? mounttype::normal : mounttype::inverted,
                    initlatency, cmdslatency, startpos, endpos);
            });

            auto iface = servo::Factory::create<Servo, config_t>(config);
            iface->moveto(servonum, pospct);
            std::cout << "Press [enter]" << std::flush;
            getchar();
            iface->movestart(servonum);
            std::cout << "Press [enter]" << std::flush;
            getchar();
            std::cout << "Second scenario DONE -> disengaging servo\n";
        }
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }
    return 0;
}
