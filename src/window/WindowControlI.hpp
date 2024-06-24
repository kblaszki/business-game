/* Created by kblaszki */
#pragma once

struct WindowControlI
{
    virtual ~WindowControlI() = default;
    virtual bool isOpen() const = 0;
    virtual void close() = 0;
};