/* Created by kblaszki */
#pragma once

#include "EventManagerI.hpp"

#include <memory>
#include <unordered_map>

template<typename T>
concept HasManagerType = requires {
    { T::MANAGER_TYPE } -> std::convertible_to<ManagerOf>;
};
template<typename T>
concept IsBaseOfEventManager = HasManagerType<T> && std::is_base_of_v<EventManager<T::MANAGER_TYPE>, T>;

class EventManagers
{
public:
    virtual ~EventManagers() = default;

    template<ManagerOf MANAGER_TYPE>
    EventManager<MANAGER_TYPE>& get()
    {

        return static_cast<EventManager<MANAGER_TYPE>&>(get(MANAGER_TYPE));
    }

protected:
    EventManagerI& get(ManagerOf type)
    {
        auto manager = eventManagers.find(type);
        if(eventManagers.end() == manager or nullptr == manager->second)
        {
            throw std::runtime_error("No event manager for this event type");
        }
        return *manager->second;
    }

    std::unordered_map<ManagerOf, std::unique_ptr<EventManagerI>> eventManagers;
};

class LordOfEventManagers : public EventManagers
{
public:
    template<typename EVENT_MANAGER, typename... Args>
    void emplace(Args&&... args)
        requires IsBaseOfEventManager<EVENT_MANAGER>
    {

        auto eventManager = std::make_unique<EVENT_MANAGER>(std::forward<Args>(args)...);
        auto manager = eventManagers.find(EVENT_MANAGER::MANAGER_TYPE);
        if(eventManagers.end() not_eq manager)
        {
            throw std::runtime_error("Event manager already exists");
        }
        eventManagers.emplace(EVENT_MANAGER::MANAGER_TYPE, std::move(eventManager));
    }

    template<typename EVENT_MANAGER>
    void emplace(std::unique_ptr<EVENT_MANAGER>&& eventManager)
        requires IsBaseOfEventManager<EVENT_MANAGER>
    {
        auto manager = eventManagers.find(EVENT_MANAGER::MANAGER_TYPE);
        if(eventManagers.end() not_eq manager)
        {
            throw std::runtime_error("Event manager already exists");
        }
        eventManagers.emplace(EVENT_MANAGER::MANAGER_TYPE, std::move(eventManager));
    }
};