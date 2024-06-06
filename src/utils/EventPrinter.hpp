/* Created by kblaszki */
#pragma once

#include <SFML/Window/Event.hpp>

#include <string>

std::string to_string(const sf::Event& event);
std::string to_string(const sf::Event::SizeEvent& event);
std::string to_string(const sf::Event::KeyEvent& event);
std::string to_string(const sf::Event::TextEvent& event);
std::string to_string(const sf::Event::MouseMoveEvent& event);
std::string to_string(const sf::Event::MouseButtonEvent& event);
std::string to_string(const sf::Event::MouseWheelEvent& event);
std::string to_string(const sf::Event::MouseWheelScrollEvent& event);
std::string to_string(const sf::Event::JoystickMoveEvent& event);
std::string to_string(const sf::Event::JoystickButtonEvent& event);
std::string to_string(const sf::Event::JoystickConnectEvent& event);
std::string to_string(const sf::Event::TouchEvent& event);
std::string to_string(const sf::Event::SensorEvent& event);
std::string to_string(sf::Event::EventType eventType);