/* Created by kblaszki */

#include "KeyReleasedManager.hpp"

void KeyReleasedManager::handleEvent(const sf::Event& event)
{
    auto specificKeyReleasedHandlers = keyReleasedHandlers.find(event.key.code);
    if(keyReleasedHandlers.end() not_eq specificKeyReleasedHandlers)
    {
        for(auto& handle: specificKeyReleasedHandlers->second)
        {
            handle(event.key);
        }
    }
}

KeyReleasedManager::KeyReleasedHandlerUnRegisterer
KeyReleasedManager::registerHandler(sf::Keyboard::Key key,
                                    KeyReleasedHandler&& handler)
{
    return keyReleasedHandlers[key].emplace(std::move(handler));
}
