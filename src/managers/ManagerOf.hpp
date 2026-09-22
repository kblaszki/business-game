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
    Joystick, // reserved — no manager yet
    Touch,    // reserved — no manager yet
    Sensor    // reserved — no manager yet
};

ManagerOf getManagerOf(const sf::Event& event);
