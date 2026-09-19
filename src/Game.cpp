/* Created by kblaszki */

#include "Game.hpp"

#include <SFML/Graphics.hpp>

#include <optional>

void Game::run()
{
    sf::RenderWindow window{sf::VideoMode{DESIGN_SIZE}, "Business game"};
    window.setFramerateLimit(60);

    while(window.isOpen())
    {
        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}
