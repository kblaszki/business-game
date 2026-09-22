/* Created by kblaszki */

#include "GameExitManager.hpp"

GameExitManager::GameExitManager(WindowCloserI& windowCloser)
    : windowCloser{windowCloser}
{
}

void GameExitManager::close()
{
    windowCloser.close();
    for(auto& handle: gameExitHandlers)
    {
        handle();
    }
}

void GameExitManager::handleEvent(const sf::Event& /*event*/)
{
    close();
}

GameExitManager::ExitUnRegisterer GameExitManager::registerExitHandler(ExitHandler&& handler)
{
    return gameExitHandlers.emplace(std::move(handler));
}