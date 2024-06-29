/* Created by kblaszki */

#include "GameExitManager.hpp"

void GameExitManager::handleEvent(const sf::Event& /*event*/)
{
    for(auto& handle: gameExitHandlers)
    {
        handle();
    }
}

GameExitManager::ExitUnRegisterer GameExitManager::registerExitHandler(ExitHandler&& handler)
{
    return gameExitHandlers.emplace(std::move(handler));
}