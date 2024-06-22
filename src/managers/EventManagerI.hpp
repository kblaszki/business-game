/* Created by kblaszki */
#pragma once

#include <SFML/Window/Event.hpp>

struct EventManagerI
{
    virtual ~EventManagerI() = default;
    virtual void handleEvent(const sf::Event& event) = 0;
};