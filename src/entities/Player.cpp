/* Created by kblaszki */

#include "Player.hpp"

Player::Player(KeyboardManagerI& keyboardManager)
{

    shape.setPointCount(4);

    shape.setPoint(0, sf::Vector2f(300, 200));  // Top-left
    shape.setPoint(1, sf::Vector2f(800, 200));  // Top-right
    shape.setPoint(2, sf::Vector2f(1000, 400)); // Bottom-right
    shape.setPoint(3, sf::Vector2f(100, 400));  // Bottom-left

    shape.setFillColor(sf::Color::Green);

    keyboardManager.registerKeyHandler(sf::Keyboard::Left, [this](const KeyStatus status, const sf::Event::KeyEvent&) {
        if(KeyStatus::Pressed == status)
        {
            moveLeft = true;
        }
        else
        {
            moveLeft = false;
        }
    });
    keyboardManager.registerKeyHandler(sf::Keyboard::Right, [this](const KeyStatus status, const sf::Event::KeyEvent&) {
        if(KeyStatus::Pressed == status)
        {
            moveRight = true;
        }
        else
        {
            moveRight = false;
        }
    });
    keyboardManager.registerKeyHandler(sf::Keyboard::Down, [this](const KeyStatus status, const sf::Event::KeyEvent&) {
        if(KeyStatus::Pressed == status)
        {
            moveDown = true;
        }
        else
        {
            moveDown = false;
        }
    });
    keyboardManager.registerKeyHandler(sf::Keyboard::Up, [this](const KeyStatus status, const sf::Event::KeyEvent&) {
        if(KeyStatus::Pressed == status)
        {
            moveUp = true;
        }
        else
        {
            moveUp = false;
        }
    });
}

void Player::update()
{
    if(moveLeft)
    {
        shape.move(-10.f, 0.f);
    }
    if(moveRight)
    {
        shape.move(10.f, 0.f);
    }
    if(moveDown)
    {
        shape.move(0.f, 10.f);
    }
    if(moveUp)
    {
        shape.move(0.f, -10.f);
    }
}

void Player::draw(DrawerI& drawer) const
{
    drawer.draw(shape);
}