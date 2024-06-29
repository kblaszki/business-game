/* Created by kblaszki */
#pragma once

#include <managers/KeyboardManagerI.hpp>
#include <window/DrawerI.hpp>

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player(KeyboardManagerI& keyboardManager);

    void update();
    void draw(DrawerI& drawer) const;

private:
    sf::ConvexShape shape{};
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
};