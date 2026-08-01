/* Created by kblaszki */

#include "MouseManager.hpp"

static sf::Event::MouseButtonPressed toCanonicalButton(const sf::Event::MouseButtonReleased& event)
{
    sf::Event::MouseButtonPressed button{};
    button.button = event.button;
    button.position = event.position;
    return button;
}

void MouseManager::handleEvent(const sf::Event& event)
{
    if(const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
    {
        handleMouseMoveEvent(*mouseMoved);
    }
    else if(const auto* buttonPressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        handleMouseButtonEvent(MouseButtonStatus::Pressed, *buttonPressed);
    }
    else if(const auto* buttonReleased = event.getIf<sf::Event::MouseButtonReleased>())
    {
        handleMouseButtonEvent(MouseButtonStatus::Released, toCanonicalButton(*buttonReleased));
    }
    else if(const auto* wheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>())
    {
        handleMouseScrollEvent(*wheelScrolled);
    }
    else if(event.is<sf::Event::MouseEntered>())
    {
        handleStatusEvent(MouseStatus::Entered);
    }
    else if(event.is<sf::Event::MouseLeft>())
    {
        handleStatusEvent(MouseStatus::Left);
    }
}

void MouseManager::handleMouseMoveEvent(const sf::Event::MouseMoved& event)
{
    for(auto& handle: mouseMoveHandlers)
    {
        handle(event);
    }
}

void MouseManager::handleMouseButtonEvent(const MouseButtonStatus status, const sf::Event::MouseButtonPressed& event)
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

void MouseManager::handleMouseScrollEvent(const sf::Event::MouseWheelScrolled& event)
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
