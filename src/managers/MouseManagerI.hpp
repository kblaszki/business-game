/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

#include <utils/ManagedList.hpp>

#include <functional>

template<>
struct EventManager<ManagerOf::Mouse> : public EventManagerI
{
    static constexpr ManagerOf MANAGER_TYPE = ManagerOf::Mouse;

    using MoveHandler = std::function<void(const sf::Event::MouseMoveEvent&)>;
    using MoveUnRegisterer = ManagedList<MoveHandler>::UnRegisterer;

    virtual MoveUnRegisterer registerMoveHandler(MoveHandler&& handler) = 0;
};

using MouseManagerI = EventManager<ManagerOf::Mouse>;