/* Created by kblaszki */
#pragma once

#include "ManagerOf.hpp"

#include <SFML/Window/Event.hpp>

struct EventManagerI
{
    virtual ~EventManagerI() = default;
    virtual void handleEvent(const sf::Event& event) = 0;
};

template<ManagerOf TYPE>
struct EventManager : public EventManagerI
{
    static constexpr ManagerOf MANAGER_TYPE = TYPE;
};