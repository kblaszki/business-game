/* Created by kblaszki */
#pragma once

#include <managers/EventManagers.hpp>
#include <managers/MouseMovedManagerI.hpp>
#include <window/DrawerI.hpp>

#include <SFML/Graphics.hpp>

class Button
{
public:
    Button(EventManagers& eventManagers,
           const std::string& text,
           sf::Vector2f position,
           sf::Vector2f size,
           sf::Color idleColor,
           sf::Color hoverColor);

    void update();
    void draw(DrawerI& drawer) const;

private:
    sf::RectangleShape shape;
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Text buttonText;
    sf::Font font;
    bool isHovered;
};