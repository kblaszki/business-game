/* Created by kblaszki */
#pragma once

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

struct WindowViewI
{
    virtual ~WindowViewI() = default;
    virtual void setView(const sf::View& view) = 0;
    virtual sf::Vector2u getSize() const = 0;
};
