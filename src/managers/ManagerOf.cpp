/* Created by kblaszki */

#include "ManagerOf.hpp"

ManagerOf getManagerOf(const sf::Event& event)
{
    if(event.is<sf::Event::Closed>())
    {
        return ManagerOf::GameExit;
    }
    if(event.is<sf::Event::FocusLost>() or event.is<sf::Event::FocusGained>() or event.is<sf::Event::Resized>())
    {
        return ManagerOf::GameWindow;
    }
    if(event.is<sf::Event::KeyPressed>() or event.is<sf::Event::KeyReleased>() or event.is<sf::Event::TextEntered>())
    {
        return ManagerOf::Keyboard;
    }
    if(event.is<sf::Event::MouseWheelScrolled>() or event.is<sf::Event::MouseButtonPressed>()
       or event.is<sf::Event::MouseButtonReleased>() or event.is<sf::Event::MouseMoved>()
       or event.is<sf::Event::MouseEntered>() or event.is<sf::Event::MouseLeft>())
    {
        return ManagerOf::Mouse;
    }
    if(event.is<sf::Event::JoystickButtonPressed>() or event.is<sf::Event::JoystickButtonReleased>()
       or event.is<sf::Event::JoystickMoved>() or event.is<sf::Event::JoystickConnected>()
       or event.is<sf::Event::JoystickDisconnected>())
    {
        return ManagerOf::Joystick;
    }
    if(event.is<sf::Event::TouchBegan>() or event.is<sf::Event::TouchMoved>() or event.is<sf::Event::TouchEnded>())
    {
        return ManagerOf::Touch;
    }
    if(event.is<sf::Event::SensorChanged>())
    {
        return ManagerOf::Sensor;
    }
    return ManagerOf::None;
}
