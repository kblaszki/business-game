/* Created by kblaszki */
#pragma once

#include "KeyReleasedManagerI.hpp"

#include <unordered_map>

class KeyReleasedManager : public EventManager<sf::Event::KeyReleased>
{
public:
    void handleEvent(const sf::Event& event) override;
    KeyReleasedHandlerUnRegisterer registerHandler(sf::Keyboard::Key key, KeyReleasedHandler&& handler) override;

private:
    std::unordered_map<sf::Keyboard::Key, ManagedList<KeyReleasedHandler>> keyReleasedHandlers;
};
