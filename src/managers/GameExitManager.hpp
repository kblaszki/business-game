/* Created by kblaszki */
#pragma once

#include "GameExitManagerI.hpp"

#include <window/WindowCloserI.hpp>

class GameExitManager : public GameExitManagerI
{
public:
    GameExitManager(WindowCloserI& windowCloser);

    void close() override;
    void handleEvent(const sf::Event& event) override;
    [[nodiscard]] ExitUnRegisterer registerExitHandler(ExitHandler&& handler) override;

private:
    ManagedList<ExitHandler> gameExitHandlers;
    WindowCloserI& windowCloser;
};
