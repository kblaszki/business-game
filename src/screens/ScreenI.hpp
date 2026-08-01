/* Created by kblaszki */
#pragma once

struct ScreenI
{
    virtual ~ScreenI() = default;
    virtual void update(float dt) = 0;
    virtual void display() = 0;
};
