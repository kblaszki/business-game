/* Created by kblaszki */
#pragma once

#include <handlers/OnClickHandler.hpp>
#include <handlers/OnHoverHandler.hpp>
#include <managers/MouseManagerI.hpp>
#include <window/DrawerI.hpp>

#include <SFML/Graphics.hpp>

class Button
    : protected OnHoverHandler
    , protected OnClickHandler
{
public:
    Button(MouseManagerI& mouseManager,
           const std::string& text,
           sf::Vector2f position,
           sf::Vector2f size,
           sf::Color idleColor,
           sf::Color hoverColor);

    void update();
    void draw(DrawerI& drawer) const;

protected:
    void onHover() override;
    void onHoverOut() override;
    void onClick() override;
    void onUnClick(bool isHovered) override;
    bool isHover(int x, int y) const override;

private:
    sf::RectangleShape shape;
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Text buttonText;
    sf::Font font;
};