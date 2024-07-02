/* Created by kblaszki */
#pragma once

struct WindowCloserI
{
    virtual ~WindowCloserI() = default;
    virtual void close() = 0;
};