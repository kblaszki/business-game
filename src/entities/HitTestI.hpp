/* Created by kblaszki */
#pragma once

#include <SFML/System/Vector2.hpp>

struct HitTestI
{
    virtual ~HitTestI() = default;
    virtual bool contains(sf::Vector2i pixel) const = 0;
};
