/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

#include <utils/ManagedList.hpp>

#include <functional>

struct ClosedEventManagerI : public EventManagerI
{
    static constexpr sf::Event::EventType EVENT_TYPE = sf::Event::Closed;

    using ClosedHandler = std::function<void()>;
    using ClosedHandlerUnRegisterer = ManagedList<ClosedHandler>::UnRegisterer;

    virtual ClosedHandlerUnRegisterer registerClosedHandler(ClosedHandler&& handler) = 0;
};