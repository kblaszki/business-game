/* Created by kblaszki */
#include "EventController.hpp"

#include <utils/EventPrinter.hpp>

#include <iostream>
#include <string>

EventController::EventController(EventCollectorI& eventCollector)
 : eventCollector{eventCollector}
 , eventHandlers{}
{
}

void EventController::handleEvents()
{
    sf::Event event{};
        while (eventCollector.pollEvent(event)) 
        {
            std::cerr << to_string(event) << std::endl;
            auto handler = eventHandlers.find(event.type);
            if(eventHandlers.end() not_eq handler)
            {
                handler->second(event);
            }
        }
}

void EventController::registerEventHandler(const sf::Event::EventType eventType, EventHandler&& handler)
{
    if(eventHandlers.cend() not_eq eventHandlers.find(eventType))
    {
        throw std::runtime_error{"Can't register event:" + std::to_string(eventType) + " handler, because it already exists!"};
    }
    eventHandlers.emplace(eventType, std::move(handler));
}

void EventController::unregisterEventHandler(const sf::Event::EventType eventType)
{
    if(eventHandlers.cend() == eventHandlers.find(eventType))
    {
        throw std::runtime_error{"Can't unregister event:" + std::to_string(eventType) + " handler, because it not exists!"};
    }
    eventHandlers.erase(eventType);
}