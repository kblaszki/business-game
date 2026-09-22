/* Created by kblaszki */
#pragma once

struct WindowOpenerI
{
    virtual ~WindowOpenerI() = default;
    virtual bool isOpen() const = 0;
};