/* Created by kblaszki */
#pragma once

#include "EventControllerI.hpp"
#include "ScreenControllerI.hpp"

#include <window/WindowI.hpp>

#include <memory>

class GameController
{
public:
    GameController(std::unique_ptr<WindowI>&& window,
                   std::unique_ptr<EventControllerI>&& eventController,
                   std::unique_ptr<ScreenControllerI>&& screenController);

    void run();

private:
    std::unique_ptr<WindowI> window;
    std::unique_ptr<EventControllerI> eventController;
    std::unique_ptr<ScreenControllerI> screenController;
};