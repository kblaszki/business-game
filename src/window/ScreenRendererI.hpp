/* Created by kblaszki */
#pragma once

#include "CleanerI.hpp"
#include "DisplayerI.hpp"
#include "DrawerI.hpp"

struct ScreenRendererI
    : public CleanerI
    , public DrawerI
    , public DisplayerI
{
};