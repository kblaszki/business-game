/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

#include <memory>
#include <unordered_map>

template<typename T>
concept HasEventType = requires {
    { T::EVENT_TYPE } -> std::convertible_to<sf::Event::EventType>;
};

class EventManagers
{
public:
    virtual ~EventManagers() = default;

    template<typename EventManager>
    EventManager& get()
        requires HasEventType<EventManager>
    {

        return static_cast<EventManager&>(get(EventManager::EVENT_TYPE));
    }

protected:
    EventManagerI& get(sf::Event::EventType eventType)
    {
        auto manager = eventManagers.find(eventType);
        if(eventManagers.end() == manager or nullptr == manager->second)
        {
            throw std::runtime_error("No event manager for this event type");
        }
        return *manager->second;
    }

    std::unordered_map<sf::Event::EventType, std::unique_ptr<EventManagerI>>
        eventManagers;
};

class LordOfEventManagers : public EventManagers
{
public:
    template<typename EventManager, typename... Args>
    void emplace(Args&&... args)
        requires HasEventType<EventManager>
    {

        auto eventManager =
            std::make_unique<EventManager>(std::forward<Args>(args)...);
        auto manager = eventManagers.find(EventManager::EVENT_TYPE);
        if(eventManagers.end() not_eq manager)
        {
            throw std::runtime_error("Event manager already exists");
        }
        eventManagers.emplace(EventManager::EVENT_TYPE,
                              std::move(eventManager));
    }

    template<typename EventManager>
    void emplace(std::unique_ptr<EventManager>&& eventManager)
        requires HasEventType<EventManager>
    {
        auto manager = eventManagers.find(EventManager::EVENT_TYPE);
        if(eventManagers.end() not_eq manager)
        {
            throw std::runtime_error("Event manager already exists");
        }
        eventManagers.emplace(EventManager::EVENT_TYPE,
                              std::move(eventManager));
    }
};