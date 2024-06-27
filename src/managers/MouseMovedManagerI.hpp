/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

#include <utils/ManagedList.hpp>

#include <functional>

template<>
struct EventManager<sf::Event::MouseMoved> : public EventManagerI
{
    static constexpr sf::Event::EventType EVENT_TYPE = sf::Event::MouseMoved;

    using MouseMovedHandler = std::function<void(const sf::Event::MouseMoveEvent&)>;
    using MouseMovedHandlerUnRegisterer = ManagedList<MouseMovedHandler>::UnRegisterer;

    virtual MouseMovedHandlerUnRegisterer registerHandler(MouseMovedHandler&& handler) = 0;
};