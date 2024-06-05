/* Created by kblaszki */
#pragma once

#include "EventController.hpp"

#include <core/WindowI.hpp>

#include <memory>

class GameController
{
public:
    GameController(std::unique_ptr<WindowI>&& gameWindow);

    void run();
private:

    std::unique_ptr<WindowI> window;
    std::unique_ptr<EventController> eventController;
};