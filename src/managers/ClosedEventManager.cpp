/* Created by kblaszki */

#include "ClosedEventManager.hpp"

void ClosedEventManager::handleEvent(const sf::Event& /*event*/)
{
    for(auto& handle: closedHandlers)
    {
        handle();
    }
}

ClosedEventManager::ClosedHandlerUnRegisterer
ClosedEventManager::registerClosedHandler(ClosedHandler&& handler)
{
    return closedHandlers.emplace(std::move(handler));
}