/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

#include <utils/ManagedList.hpp>

#include <functional>

template<>
struct EventManager<EventManagerOf::GameExit> : public EventManagerI
{
    static constexpr EventManagerOf EVENT_MANAGER_TYPE = EventManagerOf::GameExit;

    using ExitHandler = std::function<void()>;
    using ExitUnRegisterer = ManagedList<ExitHandler>::UnRegisterer;

    virtual ExitUnRegisterer registerExitHandler(ExitHandler&& handler) = 0;
};

using GameExitManagerI = EventManager<EventManagerOf::GameExit>;