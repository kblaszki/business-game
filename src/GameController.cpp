/* Created by kblaszki */

#include "GameController.hpp"
#include "WindowSFML.hpp"

GameController::GameController()
 : window{std::make_unique<WindowSFML>()}
{
}

void GameController::run()
{
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window->isOpen())
    {
        sf::Event event{};
        while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window->close();
        }
        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
            window->close();
        }

        window->clear();
        window->draw(shape);
        window->display();
    }
}