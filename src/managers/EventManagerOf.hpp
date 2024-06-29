/* Created by kblaszki */
#pragma once

#include <SFML/Window/Event.hpp>

enum class EventManagerOf
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

EventManagerOf getEventManagerOf(sf::Event::EventType event);
