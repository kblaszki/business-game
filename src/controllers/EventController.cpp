/* Created by kblaszki */
#include "EventController.hpp"

#include <managers/EventManagers.hpp>

EventController::EventController(EventCollectorI& eventCollector)
    : eventCollector{eventCollector}
{
}

void EventController::handleEvents()
{
    while(const auto event = eventCollector.pollEvent())
    {
        auto eventManager = eventManagers.find(getManagerOf(*event));
        if(eventManagers.end() not_eq eventManager)
        {
            eventManager->second->handleEvent(*event);
        }
    }
}
