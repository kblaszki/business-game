/* Created by kblaszki */
#pragma once

#include "EventControllerI.hpp"

#include <managers/EventManagers.hpp>
#include <window/EventCollectorI.hpp>

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