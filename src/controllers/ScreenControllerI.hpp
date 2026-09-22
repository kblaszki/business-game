/* Created by kblaszki */
#pragma once

#include <screens/ScreenUpdaterI.hpp>

struct ScreenControllerI : public ScreenUpdaterI
{
    virtual void update(float dt) = 0;
    virtual void display() = 0;
};