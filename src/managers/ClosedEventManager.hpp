/* Created by kblaszki */
#pragma once

#include "ClosedEventManagerI.hpp"

class ClosedEventManager : public ClosedEventManagerI
{
public:
    void handleEvent(const sf::Event& event) override;
    ClosedHandlerUnRegisterer registerClosedHandler(ClosedHandler&& handler) override;

private:
    ManagedList<ClosedHandler> closedHandlers;
};
