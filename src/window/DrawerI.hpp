/* Created by kblaszki */
#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>

struct DrawerI
{
    virtual ~DrawerI() = default;
    virtual void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default) = 0;
};
