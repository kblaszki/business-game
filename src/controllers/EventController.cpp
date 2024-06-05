/* Created by kblaszki */
#include "EventController.hpp"

#include <iostream>

EventController::EventController(EventCollectorI& eventCollector)
 : eventCollector{eventCollector}
 , eventHandlers{}
 , newId{}
{
}

void EventController::handleEvents()
{
    sf::Event event{};
        while (eventCollector.pollEvent(event)) 
        {
            std::cerr << "EVENT " << event.type << std::endl;
            auto handlers = eventHandlers.find(event.type);
            if(handlers not_eq eventHandlers.end())
            {
                for(const auto& handler : handlers->second)
                {
                    handler.second(event);
                }
            }
        }
}

std::uint32_t EventController::registerEventHandler(const sf::Event::EventType eventType, std::function<void(const sf::Event&)>&& handler)
{
    auto handlers = eventHandlers.find(eventType);
    if(handlers not_eq eventHandlers.end())
    {
        handlers->second.emplace(newId++, std::move(handler));
        return newId - 1u;
    }
    eventHandlers.emplace(eventType, std::map<std::uint32_t, std::function<void(const sf::Event&)>>{{newId++, std::move(handler)}});
    return newId - 1u;
}

void EventController::unregisterEventHandler(const sf::Event::EventType eventType, const std::uint32_t id)
{
    auto handlers = eventHandlers.find(eventType);
    if(handlers not_eq eventHandlers.end())
    {
        handlers->second.erase(id);
    }
}