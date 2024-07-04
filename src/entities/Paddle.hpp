/* Created by kblaszki */
#pragma once

#include "EntityI.hpp"

#include <managers/KeyboardManagerI.hpp>
#include <window/DrawerI.hpp>

#include <SFML/Graphics.hpp>

class Paddle : public EntityI
{
public:
    Paddle(KeyboardManagerI& keyboardManager);

    void update() override;
    void draw(DrawerI& drawer) const override;

private:
    sf::RectangleShape shape{};
    bool moveLeft = false;
    bool moveRight = false;
};