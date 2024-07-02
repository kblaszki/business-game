/* Created by kblaszki */
#pragma once

#include "EventCollectorI.hpp"
#include "ScreenRendererI.hpp"
#include "WindowCloserI.hpp"
#include "WindowOpenerI.hpp"

struct WindowI
    : public WindowOpenerI
    , public WindowCloserI
    , public ScreenRendererI
    , public EventCollectorI
{
};
