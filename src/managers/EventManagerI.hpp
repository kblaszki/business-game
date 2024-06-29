/* Created by kblaszki */
#pragma once

#include "EventManagerOf.hpp"

#include <SFML/Window/Event.hpp>

struct EventManagerI
{
    virtual ~EventManagerI() = default;
    virtual void handleEvent(const sf::Event& event) = 0;
};

template<EventManagerOf TYPE>
struct EventManager : public EventManagerI
{
    static constexpr EventManagerOf EVENT_MANAGER_TYPE = TYPE;
};