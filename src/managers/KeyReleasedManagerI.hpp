/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

#include <utils/ManagedList.hpp>

#include <functional>

struct KeyReleasedManagerI : public EventManagerI
{
    static constexpr sf::Event::EventType EVENT_TYPE = sf::Event::KeyReleased;

    using KeyReleasedHandler = std::function<void(const sf::Event::KeyEvent&)>;
    using KeyReleasedHandlerUnRegisterer =
        ManagedList<KeyReleasedHandler>::UnRegisterer;

    virtual ~KeyReleasedManagerI() = default;
    virtual KeyReleasedHandlerUnRegisterer
    registerHandler(sf::Keyboard::Key key, KeyReleasedHandler&& handler) = 0;
};