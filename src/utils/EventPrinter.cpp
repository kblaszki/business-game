/* Created by kblaszki */

#include "EventPrinter.hpp"

std::string to_string(const sf::Event& event)
{
    std::string eventType{"Event::" + to_string(event.type) + ": "};
    switch(event.type)
    {
        case sf::Event::EventType::Resized:
            return eventType + to_string(event.size);
        case sf::Event::EventType::KeyPressed:
        case sf::Event::EventType::KeyReleased:
            return eventType + to_string(event.key);
        case sf::Event::EventType::TextEntered:
            return eventType + to_string(event.text);
        case sf::Event::EventType::MouseMoved:
            return eventType + to_string(event.mouseMove);
        case sf::Event::EventType::MouseButtonPressed:
        case sf::Event::EventType::MouseButtonReleased:
            return eventType + to_string(event.mouseButton);
        case sf::Event::EventType::MouseWheelMoved:
            return eventType + to_string(event.mouseWheel);
        case sf::Event::EventType::MouseWheelScrolled:
            return eventType + to_string(event.mouseWheelScroll);
        case sf::Event::EventType::JoystickMoved:
            return eventType + to_string(event.joystickMove);
        case sf::Event::EventType::JoystickButtonPressed:
        case sf::Event::EventType::JoystickButtonReleased:
            return eventType + to_string(event.joystickButton);
        case sf::Event::EventType::JoystickConnected:
        case sf::Event::EventType::JoystickDisconnected:
            return eventType + to_string(event.joystickConnect);
        case sf::Event::EventType::TouchBegan:
        case sf::Event::EventType::TouchMoved:
        case sf::Event::EventType::TouchEnded:
            return eventType + to_string(event.touch);
        case sf::Event::EventType::SensorChanged:
            return eventType + to_string(event.sensor);
        default:
            return eventType;
    }
}

std::string to_string(const sf::Event::SizeEvent& event)
{
    return "width:" + std::to_string(event.width) + ", height:" + std::to_string(event.height);
}

std::string to_string(const sf::Event::KeyEvent& event)
{
    return "key:" + sf::Keyboard::getDescription(event.scancode) + ", isAltPressed=:" + std::to_string(event.alt)
           + ", isCtrlPressed=:" + std::to_string(event.control) + ", isShiftPressed=:" + std::to_string(event.shift)
           + ", isSystemPressed=:" + std::to_string(event.system);
}

std::string to_string(const sf::Event::TextEvent& event)
{
    return "unicode:" + std::to_string(event.unicode);
}

std::string to_string(const sf::Event::MouseMoveEvent& event)
{
    return "x:" + std::to_string(event.x) + ", y:" + std::to_string(event.y);
}

std::string to_string(const sf::Event::MouseButtonEvent& event)
{
    std::string mouseButton{};
    switch(event.button)
    {
        case sf::Mouse::Button::Left:
            mouseButton = "Left";
            break;
        case sf::Mouse::Button::Right:
            mouseButton = "Right";
            break;
        case sf::Mouse::Button::Middle:
            mouseButton = "Middle";
            break;
        case sf::Mouse::Button::XButton1:
            mouseButton = "XButton1";
            break;
        case sf::Mouse::Button::XButton2:
            mouseButton = "XButton2";
            break;
        case sf::Mouse::Button::ButtonCount:
            mouseButton = "ButtonCount";
            break;
        default:
            mouseButton = "Unknow";
    }
    return "button:" + mouseButton + ", x:" + std::to_string(event.x) + ", y:" + std::to_string(event.y);
}

std::string to_string(const sf::Event::MouseWheelEvent& event)
{
    return "delta:" + std::to_string(event.delta) + ", x:" + std::to_string(event.x) + ", y:" + std::to_string(event.y);
}

std::string to_string(const sf::Event::MouseWheelScrollEvent& event)
{
    return "wheel:"
           + std::string{sf::Mouse::Wheel::HorizontalWheel == event.wheel ? "HorizontalWheel" : "VerticalWheel"}
           + ", delta:" + std::to_string(event.delta) + ", x:" + std::to_string(event.x)
           + ", y:" + std::to_string(event.y);
}

std::string to_string(const sf::Event::JoystickMoveEvent&)
{
    return ""; // TODO: Finish it ;)
}

std::string to_string(const sf::Event::JoystickButtonEvent&)
{
    return ""; // TODO: Finish it ;)
}

std::string to_string(const sf::Event::JoystickConnectEvent&)
{
    return ""; // TODO: Finish it ;)
}

std::string to_string(const sf::Event::TouchEvent&)
{
    return ""; // TODO: Finish it ;)
}

std::string to_string(const sf::Event::SensorEvent&)
{
    return ""; // TODO: Finish it ;)
}

std::string to_string(const sf::Event::EventType eventType)
{
    switch(eventType)
    {
        case sf::Event::EventType::Closed:
            return "Closed";
        case sf::Event::EventType::Resized:
            return "Resized";
        case sf::Event::EventType::LostFocus:
            return "LostFocus";
        case sf::Event::EventType::GainedFocus:
            return "GainedFocus";
        case sf::Event::EventType::TextEntered:
            return "TextEntered";
        case sf::Event::EventType::KeyPressed:
            return "KeyPressed";
        case sf::Event::EventType::KeyReleased:
            return "KeyReleased";
        case sf::Event::EventType::MouseWheelMoved:
            return "MouseWheelMoved";
        case sf::Event::EventType::MouseWheelScrolled:
            return "MouseWheelScrolled";
        case sf::Event::EventType::MouseButtonPressed:
            return "MouseButtonPressed";
        case sf::Event::EventType::MouseButtonReleased:
            return "MouseButtonReleased";
        case sf::Event::EventType::MouseMoved:
            return "MouseMoved";
        case sf::Event::EventType::MouseEntered:
            return "MouseEntered";
        case sf::Event::EventType::MouseLeft:
            return "MouseLeft";
        case sf::Event::EventType::JoystickButtonPressed:
            return "JoystickButtonPressed";
        case sf::Event::EventType::JoystickButtonReleased:
            return "JoystickButtonReleased";
        case sf::Event::EventType::JoystickMoved:
            return "JoystickMoved";
        case sf::Event::EventType::JoystickConnected:
            return "JoystickConnected";
        case sf::Event::EventType::JoystickDisconnected:
            return "JoystickDisconnected";
        case sf::Event::EventType::TouchBegan:
            return "TouchBegan";
        case sf::Event::EventType::TouchMoved:
            return "TouchMoved";
        case sf::Event::EventType::TouchEnded:
            return "TouchEnded";
        case sf::Event::EventType::SensorChanged:
            return "SensorChanged";
        case sf::Event::EventType::Count:
            return "Count";
        default:
            return "Not expected Event!";
    }
}