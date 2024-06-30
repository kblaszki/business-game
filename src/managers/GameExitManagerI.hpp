/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

#include <utils/ManagedList.hpp>

#include <functional>

template<>
struct EventManager<ManagerOf::GameExit> : public EventManagerI
{
    static constexpr ManagerOf MANAGER_TYPE = ManagerOf::GameExit;

    using ExitHandler = std::function<void()>;
    using ExitUnRegisterer = ManagedList<ExitHandler>::UnRegisterer;

    virtual ExitUnRegisterer registerExitHandler(ExitHandler&& handler) = 0;
};

using GameExitManagerI = EventManager<ManagerOf::GameExit>;