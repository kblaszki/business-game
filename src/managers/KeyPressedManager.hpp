/* Created by kblaszki */
#pragma once

#include "KeyPressedManagerI.hpp"

#include <unordered_map>

class KeyPressedManager : public EventManager<sf::Event::KeyPressed>
{
public:
    void handleEvent(const sf::Event& event) override;
    KeyPressedHandlerUnRegisterer registerHandler(sf::Keyboard::Key key, KeyPressedHandler&& handler) override;

private:
    std::unordered_map<sf::Keyboard::Key, ManagedList<KeyPressedHandler>> keyPressedHandlers;
};
