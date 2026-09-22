#pragma once

#include <window/WindowI.hpp>

#include <SFML/System/Vector2.hpp>

#include <input/InputMapper.hpp>
#include <screen/ScreenStack.hpp>
#include <time/ClockI.hpp>
#include <time/FixedTimestep.hpp>

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
    InputMapper mapper;
    FixedTimestep timestep;
};
