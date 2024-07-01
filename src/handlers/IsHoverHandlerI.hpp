/* Created by kblaszki */
#pragma once

#include <iostream>

struct IsHoverHandlerI
{
public:
    virtual bool isHover(int x, int y) const = 0;
};