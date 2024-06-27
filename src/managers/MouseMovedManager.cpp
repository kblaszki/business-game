/* Created by kblaszki */

#include "MouseMovedManager.hpp"

void MouseMovedManager::handleEvent(const sf::Event& event)
{
    for(auto& handle: mouseMovedHandlers)
    {
        handle(event.mouseMove);
    }
}

MouseMovedManager::MouseMovedHandlerUnRegisterer MouseMovedManager::registerHandler(MouseMovedHandler&& handler)
{
    return mouseMovedHandlers.emplace(std::move(handler));
}