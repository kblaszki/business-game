/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

#include <utils/ManagedList.hpp>

#include <functional>

template<>
struct EventManager<EventManagerOf::Mouse> : public EventManagerI
{
    static constexpr EventManagerOf EVENT_MANAGER_TYPE = EventManagerOf::Mouse;

    using MoveHandler = std::function<void(const sf::Event::MouseMoveEvent&)>;
    using MoveUnRegisterer = ManagedList<MoveHandler>::UnRegisterer;

    virtual MoveUnRegisterer registerMoveHandler(MoveHandler&& handler) = 0;
};

using MouseManagerI = EventManager<EventManagerOf::Mouse>;