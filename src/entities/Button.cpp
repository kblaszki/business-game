/* Created by kblaszki */

#include "Button.hpp"

Button::Button(MouseManagerI& mouseManager,
               const std::string& text,
               sf::Vector2f position,
               sf::Vector2f size,
               sf::Color idleColor,
               sf::Color hoverColor)
    : idleColor(idleColor)
    , hoverColor(hoverColor)
    , isHovered(false)
{
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(idleColor);

    if(not font.loadFromFile("resources/fonts/prototype.ttf"))
    {
        std::cerr << "Error loading font\n";
    }

    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    buttonText.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);

    mouseManager.registerMoveHandler([this](const auto& mouseMovedEvent) {
        sf::Vector2f mousePosition(mouseMovedEvent.x, mouseMovedEvent.y);
        if(shape.getGlobalBounds().contains(mousePosition))
        {
            shape.setFillColor(this->hoverColor);
            isHovered = true;
        }
        else
        {
            shape.setFillColor(this->idleColor);
            isHovered = false;
        }
    });
}

void Button::update()
{
    // Add any additional logic here
}

void Button::draw(DrawerI& drawer) const
{
    drawer.draw(shape);
    drawer.draw(buttonText);
}
