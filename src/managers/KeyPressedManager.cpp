/* Created by kblaszki */

#include "KeyPressedManager.hpp"

void KeyPressedManager::handleEvent(const sf::Event& event)
{
    auto specificKeyPressedHandlers = keyPressedHandlers.find(event.key.code);
    if(keyPressedHandlers.end() not_eq specificKeyPressedHandlers)
    {
        for(auto& handle: specificKeyPressedHandlers->second)
        {
            handle(event.key);
        }
    }
}

KeyPressedManager::KeyPressedHandlerUnRegisterer KeyPressedManager::registerHandler(sf::Keyboard::Key key,
                                                                                    KeyPressedHandler&& handler)
{
    return keyPressedHandlers[key].emplace(std::move(handler));
}
