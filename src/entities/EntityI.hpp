/* Created by kblaszki */
#pragma once

#include <window/DrawerI.hpp>

class EntityI
{
public:
    virtual ~EntityI() = default;

    virtual void update() = 0;
    virtual void draw(DrawerI& drawer) const = 0;
};
