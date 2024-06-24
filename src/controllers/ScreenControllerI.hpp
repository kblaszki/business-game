/* Created by kblaszki */
#pragma once

struct ScreenControllerI
{
    virtual ~ScreenControllerI() = default;
    virtual void update() = 0;
    virtual void display() = 0;
};