/* Created by kblaszki */
#pragma once

#include "EntityI.hpp"

#include <handlers/OnClickHandler.hpp>
#include <handlers/OnHoverHandler.hpp>
#include <managers/MouseManagerI.hpp>
#include <window/DrawerI.hpp>

#include <SFML/Graphics.hpp>

class Button
    : public EntityI
    , protected OnHoverHandler
    , protected OnClickHandler
{
public:
    Button(MouseManagerI& mouseManager,
           const std::string& text,
           sf::Vector2f position,
           sf::Vector2f size,
           std::function<void()> onClickAction);

    void update() override;
    void draw(DrawerI& drawer) const override;

    void setOnClick(std::function<void()> onClickAction)
    {
        this->onClickAction = onClickAction;
    }

    void setFont(const std::string& fontPath)
    {
        if(not font.loadFromFile(fontPath))
        {
            std::cerr << "Error loading font\n";
        }
        buttonText.setFont(font);
    }

    void setHoverColor(sf::Color color)
    {
        this->hoverColor = color;
    }

    void setIdleColor(sf::Color color)
    {
        this->idleColor = color;
    }

protected:
    void onHover() override;
    void onHoverOut() override;
    void onClick() override;
    void onUnClick(bool isHovered) override;
    bool isHover(int x, int y) const override;

private:
    std::function<void()> onClickAction;
    sf::RectangleShape shape;
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Text buttonText;
    sf::Font font;
};