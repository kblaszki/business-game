/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

#include <utils/ManagedList.hpp>

#include <functional>

enum class MouseButtonStatus
{
    Pressed,
    Released
};

enum class MouseStatus
{
    Entered,
    Left
};

template<>
struct EventManager<ManagerOf::Mouse> : public EventManagerI
{
    static constexpr ManagerOf MANAGER_TYPE = ManagerOf::Mouse;

    using MoveHandler = std::function<void(const sf::Event::MouseMoveEvent&)>;
    using MoveUnRegisterer = ManagedList<MoveHandler>::UnRegisterer;

    using ButtonHandler = std::function<void(MouseButtonStatus, const sf::Event::MouseButtonEvent&)>;
    using ButtonUnRegisterer = ManagedList<ButtonHandler>::UnRegisterer;

    using ScrollHandler = std::function<void(const sf::Event::MouseWheelScrollEvent&)>;
    using ScrollUnRegisterer = ManagedList<ScrollHandler>::UnRegisterer;

    using StatusHandler = std::function<void(MouseStatus)>;
    using StatusUnRegisterer = ManagedList<StatusHandler>::UnRegisterer;

    virtual MoveUnRegisterer registerMoveHandler(MoveHandler&& handler) = 0;
    virtual ButtonUnRegisterer registerButtonHandler(sf::Mouse::Button button, ButtonHandler&& handler) = 0;
    virtual ScrollUnRegisterer registerScrollHandler(ScrollHandler&& handler) = 0;
    virtual StatusUnRegisterer registerStatusHandler(StatusHandler&& handler) = 0;
};

using MouseManagerI = EventManager<ManagerOf::Mouse>;