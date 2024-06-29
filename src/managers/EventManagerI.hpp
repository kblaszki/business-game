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

EventManagerOf getEventManagerOf(sf::Event::EventType event)
{
    switch(event)
    {
        case sf::Event::Closed:
            return EventManagerOf::GameExit;
        case sf::Event::LostFocus:
        case sf::Event::GainedFocus:
        case sf::Event::Resized:
            return EventManagerOf::GameWindow;
        case sf::Event::KeyPressed:
        case sf::Event::KeyReleased:
        case sf::Event::TextEntered:
            return EventManagerOf::Keyboard;
        case sf::Event::MouseWheelScrolled:
        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseButtonReleased:
        case sf::Event::MouseMoved:
        case sf::Event::MouseEntered:
        case sf::Event::MouseLeft:
            return EventManagerOf::Mouse;
        case sf::Event::JoystickButtonPressed:
        case sf::Event::JoystickButtonReleased:
        case sf::Event::JoystickMoved:
        case sf::Event::JoystickConnected:
        case sf::Event::JoystickDisconnected:
            return EventManagerOf::Joystick;
        case sf::Event::TouchBegan:
        case sf::Event::TouchMoved:
        case sf::Event::TouchEnded:
            return EventManagerOf::Touch;
        case sf::Event::SensorChanged:
            return EventManagerOf::Sensor;
        default:
            return EventManagerOf::None;
    }
}

struct EventManagerI
{
    virtual ~EventManagerI() = default;
    virtual void handleEvent(const sf::Event& event) = 0;
};

template<EventManagerOf TYPE>
struct EventManager : public EventManagerI
{
    static constexpr EventManagerOf EVENT_MANAGER_TYPE = TYPE;
};