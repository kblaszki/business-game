/* Created by kblaszki */

#include "GameController.hpp"

#include <SFML/Graphics.hpp>

GameController::GameController(std::unique_ptr<WindowI>&& gameWindow)
 : window{std::move(gameWindow)}
 , eventController{std::make_unique<EventController>(*window)}
{
    eventController->registerEventHandler(sf::Event::Closed, [this](const sf::Event&) {
        window->close();
    });
    eventController->registerEventHandler(sf::Event::KeyPressed, [this](const sf::Event& event) {
        if(sf::Keyboard::Escape == event.key.code)
        {
            window->close();
        }
    });
}

void GameController::run()
{
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window->isOpen())
    {
        eventController->handleEvents();

        window->clear();
        window->draw(shape);
        window->display();
    }
}