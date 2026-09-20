/* Created by kblaszki */
#pragma once

#include <window/DrawerI.hpp>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class GameObject
{
public:
    virtual ~GameObject() = default;

    virtual void fixedUpdate(sf::Time tick) = 0;
    virtual void draw(DrawerI& drawer) const = 0;
    virtual sf::Vector2f position() const = 0;
    virtual sf::FloatRect bounds() const = 0;
    virtual bool alive() const;
};
