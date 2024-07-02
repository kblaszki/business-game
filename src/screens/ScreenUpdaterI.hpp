/* Created by kblaszki */
#pragma once

#include "ScreenI.hpp"

#include <memory>

struct ScreenUpdaterI
{
    virtual ~ScreenUpdaterI() = default;
    virtual void setScreen(std::unique_ptr<ScreenI>&& screen) = 0;
};