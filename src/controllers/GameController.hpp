/* Created by kblaszki */
#pragma once

#include "EventControllerI.hpp"

#include <core/WindowI.hpp>

#include <memory>

class GameController
{
public:
    GameController(std::unique_ptr<WindowI>&& gameWindow, std::unique_ptr<EventControllerI>&& eventController);

    void run();

private:
    std::unique_ptr<WindowI> window;
    std::unique_ptr<EventControllerI> eventController;
};