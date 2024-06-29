/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

#include <utils/ManagedList.hpp>

#include <functional>

enum class KeyStatus
{
    Pressed,
    Released
};

template<>
struct EventManager<EventManagerOf::Keyboard> : public EventManagerI
{
    static constexpr EventManagerOf EVENT_MANAGER_TYPE = EventManagerOf::Keyboard;

    using KeyHandler = std::function<void(KeyStatus, const sf::Event::KeyEvent&)>;
    using KeyUnRegisterer = ManagedList<KeyHandler>::UnRegisterer;

    using TextHandler = std::function<void(const sf::Event::TextEvent&)>;
    using TextUnRegisterer = ManagedList<TextHandler>::UnRegisterer;

    virtual KeyUnRegisterer registerKeyHandler(sf::Keyboard::Key key, KeyHandler&& handler) = 0;
    virtual TextUnRegisterer registerTextHandler(TextHandler&& handler) = 0;
};

using KeyboardManagerI = EventManager<EventManagerOf::Keyboard>;