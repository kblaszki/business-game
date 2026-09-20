/* Created by kblaszki */
#pragma once

#include <SFML/Window/Event.hpp>

#include <optional>

struct EventCollectorI
{
    virtual ~EventCollectorI() = default;
    virtual std::optional<sf::Event> pollEvent() = 0;
};
