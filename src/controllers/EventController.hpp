/* Created by kblaszki */
#pragma once

#include "EventControllerI.hpp"

#include <core/EventCollectorI.hpp>

#include <map>

class EventController : public EventControllerI
{
public:
    EventController(EventCollectorI& eventCollector);

    void handleEvents() override;
    void registerEventHandler(sf::Event::EventType eventType,
                              EventHandler&& handler) override;
    void unregisterEventHandler(sf::Event::EventType eventType) override;

private:
    EventCollectorI& eventCollector;
    std::map<sf::Event::EventType, EventHandler> eventHandlers;
};