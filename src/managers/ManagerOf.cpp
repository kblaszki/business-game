/* Created by kblaszki */

#include "ManagerOf.hpp"

ManagerOf getManagerOf(sf::Event::EventType event)
{
    switch(event)
    {
        case sf::Event::Closed:
            return ManagerOf::GameExit;
        case sf::Event::LostFocus:
        case sf::Event::GainedFocus:
        case sf::Event::Resized:
            return ManagerOf::GameWindow;
        case sf::Event::KeyPressed:
        case sf::Event::KeyReleased:
        case sf::Event::TextEntered:
            return ManagerOf::Keyboard;
        case sf::Event::MouseWheelScrolled:
        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseButtonReleased:
        case sf::Event::MouseMoved:
        case sf::Event::MouseEntered:
        case sf::Event::MouseLeft:
            return ManagerOf::Mouse;
        case sf::Event::JoystickButtonPressed:
        case sf::Event::JoystickButtonReleased:
        case sf::Event::JoystickMoved:
        case sf::Event::JoystickConnected:
        case sf::Event::JoystickDisconnected:
            return ManagerOf::Joystick;
        case sf::Event::TouchBegan:
        case sf::Event::TouchMoved:
        case sf::Event::TouchEnded:
            return ManagerOf::Touch;
        case sf::Event::SensorChanged:
            return ManagerOf::Sensor;
        default:
            return ManagerOf::None;
    }
}