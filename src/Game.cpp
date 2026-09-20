/* Created by kblaszki */

#include "Game.hpp"

#include <window/WindowI.hpp>
#include <window/WindowSFML.hpp>

#include <SFML/System/Clock.hpp>

#include <FixedTimestep.hpp>
#include <InputMapper.hpp>
#include <MainMenuScreen.hpp>
#include <ScreenStack.hpp>
#include <memory>
#include <optional>

bool Game::handleWindowEvent(WindowI& window, ScreenStack& stack, const sf::Event& event)
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
    WindowSFML window;
    run(window);
}

void Game::run(WindowI& window)
{
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
        stack.draw(window);
        stack.applyCommands();
        if(stack.closeRequested())
        {
            window.close();
        }
        window.display();
    }
}
