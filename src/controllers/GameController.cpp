/* Created by kblaszki */

#include "GameController.hpp"

GameController::GameController(std::unique_ptr<WindowI>&& window,
                               std::unique_ptr<EventControllerI>&& eventController,
                               std::unique_ptr<ScreenControllerI>&& screenController)
    : window{std::move(window)}
    , eventController{std::move(eventController)}
    , screenController{std::move(screenController)}
{
}

void GameController::run()
{
    while(window->isOpen())
    {
        eventController->handleEvents();

        screenController->update();
        screenController->display();
    }
}