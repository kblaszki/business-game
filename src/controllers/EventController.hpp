/* Created by kblaszki */
#pragma once

#include <core/WindowI.hpp>

#include <functional>
#include <map>


class EventController
{
public:
    EventController(EventCollectorI& eventCollector);

    void handleEvents();
    std::uint32_t registerEventHandler(sf::Event::EventType eventType, std::function<void(const sf::Event&)>&& handler);
    void unregisterEventHandler(sf::Event::EventType eventType, std::uint32_t id);

private:
    EventCollectorI& eventCollector;
    std::map<sf::Event::EventType, std::map<std::uint32_t, std::function<void(const sf::Event&)>>> eventHandlers;
    std::uint32_t newId;
};