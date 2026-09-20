/* Created by kblaszki */

#include "Game.hpp"

#include <SFML/Graphics.hpp>

#include <FixedTimestep.hpp>
#include <InputMapper.hpp>
#include <MainMenuScreen.hpp>
#include <ScreenStack.hpp>
#include <SfmlDrawer.hpp>
#include <memory>
#include <optional>

bool Game::handleWindowEvent(sf::RenderWindow& window, ScreenStack& stack, const sf::Event& event)
{
    if(event.is<sf::Event::Closed>())
    {
        window.close();
        return true;
    }

    if(event.is<sf::Event::Resized>())
    {
        return true;
    }

    if(event.is<sf::Event::FocusLost>())
    {
        stack.requestPauseOverlay();
        return true;
    }

    if(event.is<sf::Event::FocusGained>())
    {
        return true;
    }

    return false;
}

void Game::run()
{
    sf::RenderWindow window{sf::VideoMode{DESIGN_SIZE}, "Business game"};
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    sf::Clock clock;
    FixedTimestep timestep;
    InputMapper mapper;
    ScreenStack stack;
    stack.requestPush(std::make_unique<MainMenuScreen>(stack));
    stack.applyCommands();

    while(window.isOpen())
    {
        while(const std::optional event = window.pollEvent())
        {
            if(handleWindowEvent(window, stack, *event))
            {
                continue;
            }

            if(const auto action = mapper.mapEvent(*event))
            {
                stack.handleAction(*action);
            }
            else
            {
                stack.handleEvent(*event);
            }
        }

        if(stack.pauseIsTop())
        {
            clock.restart();
        }
        else
        {
            const auto ticks = timestep.drain(clock.restart());
            for(auto i = 0u; i < ticks; ++i)
            {
                stack.update(FixedTimestep::tick);
            }
        }

        window.clear();
        {
            SfmlDrawer drawer{window};
            stack.draw(drawer);
        }
        stack.applyCommands();
        if(stack.closeRequested())
        {
            window.close();
        }
        window.display();
    }
}
