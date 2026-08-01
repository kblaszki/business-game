/* Created by kblaszki */

#include "KeyboardManager.hpp"

static sf::Event::KeyPressed toCanonicalKey(const sf::Event::KeyReleased& event)
{
    sf::Event::KeyPressed key{};
    key.code = event.code;
    key.scancode = event.scancode;
    key.alt = event.alt;
    key.control = event.control;
    key.shift = event.shift;
    key.system = event.system;
    return key;
}

void KeyboardManager::handleEvent(const sf::Event& event)
{
    if(const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        handleKeyEvent(KeyStatus::Pressed, *keyPressed);
    }
    else if(const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
    {
        handleKeyEvent(KeyStatus::Released, toCanonicalKey(*keyReleased));
    }
    else if(const auto* textEntered = event.getIf<sf::Event::TextEntered>())
    {
        handleTextEvent(*textEntered);
    }
}

void KeyboardManager::handleKeyEvent(const KeyStatus status, const sf::Event::KeyPressed& event)
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

void KeyboardManager::handleTextEvent(const sf::Event::TextEntered& event)
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
