#pragma once

#include <screen/ScreenStack.hpp>
#include <time/ClockI.hpp>
#include <time/FixedTimestep.hpp>
#include <window/WindowI.hpp>

#include <SFML/System/Vector2.hpp>

class Game
{
public:
    static const sf::Vector2u DESIGN_SIZE;

    explicit Game(WindowI& window, ClockI& clock, ScreenStack& screens);

    void run();

private:
    WindowI& window;
    ClockI& clock;
    ScreenStack& screens;
    FixedTimestep timestep;
};
