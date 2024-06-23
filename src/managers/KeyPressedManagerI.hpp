/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

#include <utils/ManagedList.hpp>

#include <functional>

struct KeyPressedManagerI : public EventManagerI
{
    static constexpr sf::Event::EventType EVENT_TYPE = sf::Event::KeyPressed;

    using KeyPressedHandler = std::function<void(const sf::Event::KeyEvent&)>;
    using KeyPressedHandlerUnRegisterer =
        ManagedList<KeyPressedHandler>::UnRegisterer;

    virtual ~KeyPressedManagerI() = default;
    virtual KeyPressedHandlerUnRegisterer
    registerHandler(sf::Keyboard::Key key, KeyPressedHandler&& handler) = 0;
};