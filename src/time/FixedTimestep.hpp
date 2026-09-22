#pragma once

#include <SFML/System/Time.hpp>

#include <cstdint>

struct FixedTimestep
{
    static constexpr sf::Time tick{sf::seconds(1.f / 60.f)};
    static constexpr sf::Time accumulatorMax{sf::seconds(0.25f)};

    std::uint32_t drain(sf::Time frameDt)
    {
        accumulator += frameDt;
        if(accumulator > accumulatorMax)
        {
            accumulator = accumulatorMax;
        }

        std::uint32_t steps{0};
        while(accumulator >= tick)
        {
            accumulator -= tick;
            ++steps;
        }
        return steps;
    }

    sf::Time accumulator{};
};
