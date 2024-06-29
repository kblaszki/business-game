/* Created by kblaszki */

#include "EventManagerOf.hpp"

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