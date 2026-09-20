/* Created by kblaszki */
#pragma once

#include <window/DrawerI.hpp>

#include <SFML/System/Time.hpp>

class EntityI
{
public:
    virtual ~EntityI() = default;

    virtual void fixedUpdate(sf::Time tick) = 0;
    virtual void draw(DrawerI& drawer) const = 0;
    virtual bool alive() const
    {
        return true;
    }
};
