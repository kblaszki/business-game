#pragma once

#include <screen/ScreenI.hpp>
#include <time/ClockI.hpp>
#include <time/FixedTimestep.hpp>
#include <window/WindowI.hpp>

#include <SFML/System/Vector2.hpp>

class Game
{
public:
    static const sf::Vector2u DESIGN_SIZE;

    explicit Game(WindowI& window, ClockI& clock, ScreenI& screen);

    void run();

private:
    WindowI& window;
    ClockI& clock;
    ScreenI& screen;
    FixedTimestep timestep;
};
