#include "Game.hpp"

#include <cstdint>

const sf::Vector2u Game::DESIGN_SIZE{1280u, 720u};

Game::Game(WindowI& window, ClockI& clock, ScreenI& screen)
    : window{window}
    , clock{clock}
    , screen{screen}
{
}

void Game::run()
{
    while(window.isOpen())
    {
        const sf::Time dt = clock.restart();

        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else
            {
                screen.handleEvent(*event);
            }
        }

        if(!screen.blocksUpdate())
        {
            const std::uint32_t steps = timestep.drain(dt);
            for(std::uint32_t i{0}; i < steps; ++i)
            {
                screen.update(FixedTimestep::tick);
            }
        }

        window.clear();
        screen.draw(window);
        window.display();
    }
}
