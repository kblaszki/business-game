/* Created by kblaszki */

#include "Button.hpp"

Button::Button(MouseManagerI& mouseManager,
               const std::string& text,
               sf::Vector2f position,
               sf::Vector2f size,
               sf::Color idleColor,
               sf::Color hoverColor)
    : OnHoverHandler(mouseManager)
    , OnClickHandler(mouseManager, sf::Mouse::Button::Left)
    , idleColor(idleColor)
    , hoverColor(hoverColor)
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

void Button::onHover()
{
    shape.setFillColor(this->hoverColor);
}

void Button::onHoverOut()
{
    shape.setFillColor(this->idleColor);
}

void Button::onClick()
{
    shape.setFillColor(sf::Color::Black);
}

void Button::onUnClick(bool isHovered)
{
    if(isHovered)
    {
        shape.setFillColor(sf::Color::Cyan);
    }
}

bool Button::isHover(int x, int y) const
{
    return shape.getGlobalBounds().contains(x, y);
}
