/* Created by kblaszki */
#pragma once

#include "MouseMovedManagerI.hpp"

class MouseMovedManager : public EventManager<sf::Event::MouseMoved>
{
public:
    void handleEvent(const sf::Event& event) override;
    MouseMovedHandlerUnRegisterer registerHandler(MouseMovedHandler&& handler) override;

private:
    ManagedList<MouseMovedHandler> mouseMovedHandlers;
};