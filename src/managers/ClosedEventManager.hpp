/* Created by kblaszki */
#pragma once

#include "ClosedEventManagerI.hpp"

class ClosedEventManager : public EventManager<sf::Event::Closed>
{
public:
    void handleEvent(const sf::Event& event) override;
    ClosedHandlerUnRegisterer registerHandler(ClosedHandler&& handler) override;

private:
    ManagedList<ClosedHandler> closedHandlers;
};
