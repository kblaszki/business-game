/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

#include <utils/ManagedList.hpp>

#include <functional>

template<>
struct EventManager<sf::Event::Closed> : public EventManagerI
{
    static constexpr sf::Event::EventType EVENT_TYPE = sf::Event::Closed;

    using ClosedHandler = std::function<void()>;
    using ClosedHandlerUnRegisterer = ManagedList<ClosedHandler>::UnRegisterer;

    virtual ClosedHandlerUnRegisterer registerHandler(ClosedHandler&& handler) = 0;
};