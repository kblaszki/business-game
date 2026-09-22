#include "Game.hpp"

#include <cstdint>

const sf::Vector2u Game::DESIGN_SIZE{1280u, 720u};

Game::Game(WindowI& window, ClockI& clock, ScreenStack& screens)
    : window{window}
    , clock{clock}
    , screens{screens}
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
            else if(const auto action = mapper.mapEvent(*event))
            {
                screens.handleAction(*action);
            }
            else
            {
                screens.handleEvent(*event);
            }
        }

        if(!screens.blocksUpdate())
        {
            const std::uint32_t steps = timestep.drain(dt);
            for(std::uint32_t i{0}; i < steps; ++i)
            {
                screens.update(FixedTimestep::tick);
            }
        }
        else
        {
            screens.update(FixedTimestep::tick);
        }

        window.clear();
        screens.draw(window);
        window.display();
    }
}
