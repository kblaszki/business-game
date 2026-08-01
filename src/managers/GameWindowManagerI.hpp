/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

template<>
struct EventManager<ManagerOf::GameWindow> : public EventManagerI
{
    static constexpr ManagerOf MANAGER_TYPE = ManagerOf::GameWindow;
};

using GameWindowManagerI = EventManager<ManagerOf::GameWindow>;
