#pragma once

#include <SFML/Graphics/Drawable.hpp>

class DrawerI
{
public:
    virtual ~DrawerI() = default;

    virtual void draw(const sf::Drawable& drawable) = 0;
};
