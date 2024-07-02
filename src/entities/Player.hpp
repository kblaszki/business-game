/* Created by kblaszki */
#pragma once

#include "EntityI.hpp"

#include <managers/KeyboardManagerI.hpp>
#include <window/DrawerI.hpp>

#include <SFML/Graphics.hpp>

class Player : public EntityI
{
public:
    Player(KeyboardManagerI& keyboardManager);

    void update() override;
    void draw(DrawerI& drawer) const override;

private:
    sf::ConvexShape shape{};
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
};