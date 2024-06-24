/* Created by kblaszki */
#pragma once

#include "EventCollectorI.hpp"
#include "ScreenRendererI.hpp"
#include "WindowControlI.hpp"

struct WindowI
    : public WindowControlI
    , public ScreenRendererI
    , public EventCollectorI
{
};
