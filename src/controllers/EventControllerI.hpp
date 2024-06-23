/* Created by kblaszki */
#pragma once

#include <SFML/Window/Event.hpp>

#include <memory>

struct EventControllerI
{
    virtual ~EventControllerI() = default;
    virtual void handleEvents() = 0;
};