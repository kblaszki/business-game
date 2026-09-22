/* Created by kblaszki */
#pragma once

#include "ScreenI.hpp"

#include <memory>

struct ScreenUpdaterI
{
    virtual ~ScreenUpdaterI() = default;
    virtual void pushScreen(std::unique_ptr<ScreenI>&& screen) = 0;
    virtual void popScreen() = 0;
    virtual void replaceScreen(std::unique_ptr<ScreenI>&& screen) = 0;
};
