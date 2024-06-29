/* Created by kblaszki */
#pragma once

#include "GameExitManagerI.hpp"

class GameExitManager : public GameExitManagerI
{
public:
    void handleEvent(const sf::Event& event) override;
    ExitUnRegisterer registerExitHandler(ExitHandler&& handler) override;

private:
    ManagedList<ExitHandler> gameExitHandlers;
};
