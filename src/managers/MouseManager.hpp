/* Created by kblaszki */
#pragma once

#include "MouseManagerI.hpp"

class MouseManager : public MouseManagerI
{
public:
    void handleEvent(const sf::Event& event) override;
    MoveUnRegisterer registerMoveHandler(MoveHandler&& handler) override;
    ButtonUnRegisterer registerButtonHandler(sf::Mouse::Button button, ButtonHandler&& handler) override;
    ScrollUnRegisterer registerScrollHandler(ScrollHandler&& handler) override;
    StatusUnRegisterer registerStatusHandler(StatusHandler&& handler) override;

private:
    void handleMouseMoveEvent(const sf::Event::MouseMoveEvent& event);
    void handleMouseButtonEvent(MouseButtonStatus status, const sf::Event::MouseButtonEvent& event);
    void handleMouseScrollEvent(const sf::Event::MouseWheelScrollEvent& event);
    void handleStatusEvent(MouseStatus status);

    std::unordered_map<sf::Mouse::Button, ManagedList<ButtonHandler>> mouseButtonHandlers;
    ManagedList<MoveHandler> mouseMoveHandlers;
    ManagedList<ScrollHandler> mouseScrollHandlers;
    ManagedList<StatusHandler> mouseStatusHandlers;
};