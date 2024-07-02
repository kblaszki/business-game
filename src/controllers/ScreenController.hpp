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
    void setScreen(std::unique_ptr<ScreenI>&& screen);
    void update() override;
    void display() override;

private:
    std::unique_ptr<ScreenI> currentScreen;
    std::unique_ptr<ScreenI> newScreen{nullptr};
    EventManagers& eventManagers;
    ScreenRendererI& screenRenderer;
};