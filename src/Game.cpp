#include "Game.hpp"

const sf::Vector2u Game::DESIGN_SIZE{1280u, 720u};

Game::Game(WindowI& window)
    : window{window}
{
}

void Game::run()
{
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
