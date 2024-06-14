/* Created by kblaszki */
#pragma once

#include <SFML/Window/Event.hpp>

#include <cstdint>
#include <functional>

struct KeyEventControllerI
{
    using KeyHandler = std::function<void(const sf::Event::KeyEvent&)>;

    virtual ~KeyEventControllerI() = default;
    virtual void registerKeyPressedHandler(sf::Keyboard::Key key,
                                           KeyHandler&& handler) = 0;
    virtual void registerKeyReleasedHandler(sf::Keyboard::Key key,
                                            KeyHandler&& handler) = 0;
    virtual void unregisterKeyPressedHandler(sf::Keyboard::Key key) = 0;
    virtual void unregisterKeyReleasedHandler(sf::Keyboard::Key key) = 0;
};