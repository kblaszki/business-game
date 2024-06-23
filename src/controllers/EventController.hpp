/* Created by kblaszki */
#pragma once

#include "EventControllerI.hpp"

#include <core/EventCollectorI.hpp>
#include <managers/EventManagers.hpp>

class EventController
    : public EventControllerI
    , public LordOfEventManagers
{
public:
    EventController(EventCollectorI& eventCollector);

    void handleEvents() override;

private:
    EventCollectorI& eventCollector;
};