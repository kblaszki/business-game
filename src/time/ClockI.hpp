#pragma once

#include <SFML/System/Time.hpp>

class ClockI
{
public:
    virtual ~ClockI() = default;

    virtual sf::Time restart() = 0;
    virtual sf::Time getElapsedTime() const = 0;
};
