/* Created by kblaszki */
#pragma once

#include "ScreenControllerI.hpp"

#include <managers/EventManagers.hpp>
#include <screens/ScreenI.hpp>
#include <window/ScreenRendererI.hpp>

#include <memory>

class ScreenController : public ScreenControllerI
{
public:
    ScreenController(EventManagers& eventManagers, ScreenRendererI& screenRenderer);
    void update() override;
    void display() override;

private:
    std::unique_ptr<ScreenI> currentScreen;
    EventManagers& eventManagers;
    ScreenRendererI& screenRenderer;
};