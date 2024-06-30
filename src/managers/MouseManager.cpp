/* Created by kblaszki */

#include "MouseManager.hpp"

void MouseManager::handleEvent(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::MouseMoved:
            return handleMouseMoveEvent(event.mouseMove);
        case sf::Event::MouseButtonPressed:
            return handleMouseButtonEvent(MouseButtonStatus::Pressed, event.mouseButton);
        case sf::Event::MouseButtonReleased:
            return handleMouseButtonEvent(MouseButtonStatus::Released, event.mouseButton);
        case sf::Event::MouseWheelScrolled:
            return handleMouseScrollEvent(event.mouseWheelScroll);
        case sf::Event::MouseEntered:
            return handleStatusEvent(MouseStatus::Entered);
        case sf::Event::MouseLeft:
            return handleStatusEvent(MouseStatus::Left);
        default:
            break;
    }
}

void MouseManager::handleMouseMoveEvent(const sf::Event::MouseMoveEvent& event)
{
    for(auto& handle: mouseMoveHandlers)
    {
        handle(event);
    }
}

void MouseManager::handleMouseButtonEvent(const MouseButtonStatus status, const sf::Event::MouseButtonEvent& event)
{
    auto specificMouseButtonHandlers = mouseButtonHandlers.find(event.button);
    if(mouseButtonHandlers.end() not_eq specificMouseButtonHandlers)
    {
        for(auto& handle: specificMouseButtonHandlers->second)
        {
            handle(status, event);
        }
    }
}

void MouseManager::handleMouseScrollEvent(const sf::Event::MouseWheelScrollEvent& event)
{
    for(auto& handle: mouseScrollHandlers)
    {
        handle(event);
    }
}

void MouseManager::handleStatusEvent(const MouseStatus status)
{
    for(auto& handle: mouseStatusHandlers)
    {
        handle(status);
    }
}

MouseManager::MoveUnRegisterer MouseManager::registerMoveHandler(MoveHandler&& handler)
{
    return mouseMoveHandlers.emplace(std::move(handler));
}

MouseManager::ButtonUnRegisterer MouseManager::registerButtonHandler(sf::Mouse::Button button, ButtonHandler&& handler)
{
    return mouseButtonHandlers[button].emplace(std::move(handler));
}

MouseManager::ScrollUnRegisterer MouseManager::registerScrollHandler(ScrollHandler&& handler)
{
    return mouseScrollHandlers.emplace(std::move(handler));
}

MouseManager::StatusUnRegisterer MouseManager::registerStatusHandler(StatusHandler&& handler)
{
    return mouseStatusHandlers.emplace(std::move(handler));
}
