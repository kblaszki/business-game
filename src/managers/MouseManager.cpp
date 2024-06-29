/* Created by kblaszki */

#include "MouseManager.hpp"

void MouseManager::handleEvent(const sf::Event& event)
{
    if(sf::Event::MouseMoved not_eq event.type)
    {
        return;
    }
    for(auto& handle: mouseMoveHandlers)
    {
        handle(event.mouseMove);
    }
}

MouseManager::MoveUnRegisterer MouseManager::registerMoveHandler(MoveHandler&& handler)
{
    return mouseMoveHandlers.emplace(std::move(handler));
}