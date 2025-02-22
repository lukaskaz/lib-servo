#include "servo/interfaces/rpi5/pca9685/servo.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>

int main(int argc, char** argv)
{
    try
    {
        if (argc == 3)
        {
            std::cout
                << "First scenario -> moving all servos to given position\n";
            using namespace servo::rpi5::pca9685;
            auto amount = (uint8_t)atoi(argv[1]);
            auto pospct = (uint8_t)atoi(argv[2]);
            auto startpos{800000}, endpos{2000000};
            auto sequence = std::views::iota(0, (int32_t)amount);
            config_t config;

            std::ranges::for_each(sequence, [&](uint8_t num) {
                config.emplace_back(num % 2 == 0 ? mounttype::normal
                                                 : mounttype::inverted,
                                    startpos, endpos);
            });

            auto iface = servo::Factory::create<Servo, config_t>(config);
            iface->moveto(pospct);
            // sleep(2);
            getchar();
        }
        if (argc == 4)
        {
            std::cout << "Second scenario -> moving single servo to given "
                         "position\n";
            using namespace servo::rpi5::pca9685;
            auto amount = (uint8_t)atoi(argv[1]);
            auto pospct = (uint8_t)atoi(argv[2]);
            auto servonum = (uint8_t)atoi(argv[3]);
            auto startpos{800000}, endpos{2000000};
            auto sequence = std::views::iota(0, (int32_t)amount);
            config_t config;

            std::ranges::for_each(sequence, [&](uint8_t num) {
                config.emplace_back(num % 2 == 0 ? mounttype::normal
                                                 : mounttype::inverted,
                                    startpos, endpos);
            });

            auto iface = servo::Factory::create<Servo, config_t>(config);
            iface->moveto(servonum, pospct);
            // sleep(2);
            getchar();
        }
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }
    return 0;
}
