/* Created by kblaszki */
#pragma once

#include <SFML/Window/Event.hpp>

struct EventCollectorI
{
    virtual ~EventCollectorI() = default;
    virtual bool pollEvent(sf::Event& event) = 0;
};
