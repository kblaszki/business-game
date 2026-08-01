/* Created by kblaszki */

#include "Button.hpp"

Button::Button(MouseManagerI& mouseManager,
               const std::string& text,
               sf::Vector2f position,
               sf::Vector2f size,
               std::function<void()> onClickAction)
    : OnHoverHandler(mouseManager)
    , OnClickHandler(mouseManager, sf::Mouse::Button::Left)
    , onClickAction{onClickAction}
    , idleColor{sf::Color::Red}
    , hoverColor{sf::Color::Green}
    , buttonText{font}
{
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(idleColor);

    if(not font.openFromFile("resources/fonts/prototype.ttf"))
    {
        std::cerr << "Error loading font\n";
    }

    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.position + textRect.size / 2.0f);
    buttonText.setPosition(position + size / 2.0f);
}

void Button::update(float /*dt*/) {}

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
    if(onClickAction)
    {
        onClickAction();
    }
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
    return shape.getGlobalBounds().contains(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));
}
