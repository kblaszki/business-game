/* Created by kblaszki */

#include "KeyboardManager.hpp"

void KeyboardManager::handleEvent(const sf::Event& event)
{
    switch(event.type)
    {
        case sf::Event::KeyPressed:
            return handleKeyEvent(KeyStatus::Pressed, event.key);
        case sf::Event::KeyReleased:
            return handleKeyEvent(KeyStatus::Released, event.key);
        case sf::Event::TextEntered:
            return handleTextEvent(event.text);
        default:
            break;
    }
}

void KeyboardManager::handleKeyEvent(const KeyStatus status, const sf::Event::KeyEvent& event)
{
    auto specificKeyHandlers = keyHandlers.find(event.code);
    if(keyHandlers.end() not_eq specificKeyHandlers)
    {
        for(auto& handle: specificKeyHandlers->second)
        {
            handle(status, event);
        }
    }
}

void KeyboardManager::handleTextEvent(const sf::Event::TextEvent& event)
{
    for(auto& handle: textHandlers)
    {
        handle(event);
    }
}

KeyboardManager::KeyUnRegisterer KeyboardManager::registerKeyHandler(sf::Keyboard::Key key, KeyHandler&& handler)
{
    return keyHandlers[key].emplace(std::move(handler));
}

KeyboardManager::TextUnRegisterer KeyboardManager::registerTextHandler(TextHandler&& handler)
{
    return textHandlers.emplace(std::move(handler));
}
