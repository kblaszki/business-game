/* Created by kblaszki */
#pragma once

struct DisplayerI
{
    virtual ~DisplayerI() = default;
    virtual void display() = 0;
};
