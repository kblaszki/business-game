/* Created by kblaszki */
#pragma once

#include "EntityI.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

class CollidableI : public EntityI
{
public:
    virtual sf::Vector2f position() const = 0;
    virtual sf::FloatRect bounds() const = 0;
};
