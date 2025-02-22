#include "ledrgb/interfaces/rpi5/ws281x/ledrgb.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>

int main(int argc, char** argv)
{
    using namespace ledrgb::rpi5::ws281x;

    if (argc > 2)
    {
        auto amount = (uint8_t)atoi(argv[1]);
        auto bright = (uint8_t)atoi(argv[2]);
        auto iface = ledrgb::Factory::create<Ledrgb, config_t, param_t>(
            {amount, bright});

        auto sequence = std::views::iota(0, (int32_t)amount);
        std::ranges::for_each(sequence, [iface](uint8_t num) {
            iface->light(num, (ledrgb::colortype)num);
        });
        sleep(2);
        std::cout << "Completed first scenario" << std::endl;

        ledrgb::grouptype group;
        std::ranges::for_each(
            sequence, [iface, color = amount, &group](uint8_t num) mutable {
                group.emplace_back(num, (ledrgb::colortype)(--color));
            });
        iface->light(group);
        sleep(2);
        std::cout << "Completed second scenario" << std::endl;
    }

    if (argc > 4)
    {
        // 10ms delay to restore ws281x driver after previous release
        usleep(10 * 1000);
        auto amount = (uint8_t)atoi(argv[1]);
        auto bright = (uint8_t)atoi(argv[2]);
        auto num = (uint8_t)atoi(argv[3]);
        auto color = (uint8_t)atoi(argv[4]);
        auto iface = ledrgb::Factory::create<Ledrgb, config_t, param_t>(
            {amount, bright});

        iface->light(num, (ledrgb::colortype)color, 2000ms);
        sleep(2);
        std::cout << "Light off" << std::endl;
        sleep(4);
        std::cout << "Completed third scenario" << std::endl;
    }
    return 0;
}
