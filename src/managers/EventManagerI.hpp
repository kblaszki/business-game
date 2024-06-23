/* Created by kblaszki */
#pragma once

#include <SFML/Window/Event.hpp>

struct EventManagerI
{
    virtual ~EventManagerI() = default;
    virtual void handleEvent(const sf::Event& event) = 0;
};

template<sf::Event::EventType EventType>
struct EventManager : public EventManagerI
{
    static constexpr sf::Event::EventType EVENT_TYPE = EventType;
};