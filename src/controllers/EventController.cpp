/* Created by kblaszki */
#include "EventController.hpp"

#include <managers/EventManagers.hpp>
#include <utils/EventPrinter.hpp>

#include <iostream>
#include <string>

EventController::EventController(EventCollectorI& eventCollector)
    : eventCollector{eventCollector}
{
}

void EventController::handleEvents()
{
    sf::Event event{};
    while(eventCollector.pollEvent(event))
    {
        std::cerr << to_string(event) << std::endl;
        auto eventManager = eventManagers.find(getManagerOf(event.type));
        if(eventManagers.end() not_eq eventManager)
        {
            eventManager->second->handleEvent(event);
        }
    }
}