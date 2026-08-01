/* Created by kblaszki */
#pragma once

#include <SFML/Window/Event.hpp>

enum class ManagerOf
{
    None,
    GameExit,
    GameWindow,
    Keyboard,
    Mouse,
    Joystick,
    Touch,
    Sensor
};

ManagerOf getManagerOf(const sf::Event& event);
