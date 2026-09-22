#include "ClockSFML.hpp"

sf::Time ClockSFML::restart()
{
    return clock.restart();
}

sf::Time ClockSFML::getElapsedTime() const
{
    return clock.getElapsedTime();
}
