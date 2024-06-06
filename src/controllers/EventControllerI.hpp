/* Created by kblaszki */
#pragma once

#include <SFML/Window/Event.hpp>

#include <cstdint>
#include <functional>

struct EventControllerI
{
    using EventHandler = std::function<void(const sf::Event&)>;
    using EventHandlerId = std::uint32_t;

    virtual ~EventControllerI() = default;
    virtual void handleEvents() = 0;
    virtual void registerEventHandler(sf::Event::EventType eventType,
                                      EventHandler&& handler) = 0;
    virtual void unregisterEventHandler(sf::Event::EventType eventType) = 0;
};