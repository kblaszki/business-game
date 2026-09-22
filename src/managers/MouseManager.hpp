/* Created by kblaszki */
#pragma once

#include "MouseManagerI.hpp"

class MouseManager : public MouseManagerI
{
public:
    void handleEvent(const sf::Event& event) override;
    [[nodiscard]] MoveUnRegisterer registerMoveHandler(MoveHandler&& handler) override;
    [[nodiscard]] ButtonUnRegisterer registerButtonHandler(sf::Mouse::Button button, ButtonHandler&& handler) override;
    [[nodiscard]] ScrollUnRegisterer registerScrollHandler(ScrollHandler&& handler) override;
    [[nodiscard]] StatusUnRegisterer registerStatusHandler(StatusHandler&& handler) override;

private:
    void handleMouseMoveEvent(const sf::Event::MouseMoved& event);
    void handleMouseButtonEvent(MouseButtonStatus status, const sf::Event::MouseButtonPressed& event);
    void handleMouseScrollEvent(const sf::Event::MouseWheelScrolled& event);
    void handleStatusEvent(MouseStatus status);

    std::unordered_map<sf::Mouse::Button, ManagedList<ButtonHandler>> mouseButtonHandlers;
    ManagedList<MoveHandler> mouseMoveHandlers;
    ManagedList<ScrollHandler> mouseScrollHandlers;
    ManagedList<StatusHandler> mouseStatusHandlers;
};