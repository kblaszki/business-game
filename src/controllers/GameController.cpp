/* Created by kblaszki */

#include "GameController.hpp"

GameController::GameController(
    std::unique_ptr<WindowI>&& gameWindow,
    std::unique_ptr<EventControllerI>&& gameEventController)
    : window{std::move(gameWindow)}
    , eventController{std::move(gameEventController)}
{
}

void GameController::run()
{
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while(window->isOpen())
    {
        eventController->handleEvents();

        window->clear();
        window->draw(shape);
        window->display();
    }
}