#pragma once

#include "ClockI.hpp"

#include <SFML/System/Clock.hpp>

class ClockSFML : public ClockI
{
public:
    sf::Time restart() override;
    sf::Time getElapsedTime() const override;

private:
    sf::Clock clock;
};
