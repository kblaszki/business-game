/* Created by kblaszki */
#pragma once

#include <SFML/Graphics/Color.hpp>

struct CleanerI
{
    virtual ~CleanerI() = default;
    virtual void clear(const sf::Color& color = sf::Color::Black) = 0;
};
