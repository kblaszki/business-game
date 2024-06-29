/* Created by kblaszki */
#pragma once

#include "KeyboardManagerI.hpp"

#include <unordered_map>

class KeyboardManager : public KeyboardManagerI
{
public:
    void handleEvent(const sf::Event& event) override;
    KeyUnRegisterer registerKeyHandler(sf::Keyboard::Key key, KeyHandler&& handler) override;
    TextUnRegisterer registerTextHandler(TextHandler&& handler) override;

private:
    void handleKeyEvent(KeyStatus status, const sf::Event::KeyEvent& event);
    void handleTextEvent(const sf::Event::TextEvent& event);

    std::unordered_map<sf::Keyboard::Key, ManagedList<KeyHandler>> keyHandlers;
    ManagedList<TextHandler> textHandlers;
};
