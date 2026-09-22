/* Created by kblaszki */
#pragma once

#include "EventControllerI.hpp"
#include "ScreenControllerI.hpp"

#include <window/WindowI.hpp>

#include <functional>
#include <memory>

class GameController
{
public:
    using FrameTimeProvider = std::function<float()>;

    GameController(std::unique_ptr<WindowI>&& window,
                   std::unique_ptr<EventControllerI>&& eventController,
                   std::unique_ptr<ScreenControllerI>&& screenController,
                   FrameTimeProvider frameTimeProvider = {});

    void run();

private:
    std::unique_ptr<WindowI> window;
    std::unique_ptr<EventControllerI> eventController;
    std::unique_ptr<ScreenControllerI> screenController;
    FrameTimeProvider frameTimeProvider;
};
