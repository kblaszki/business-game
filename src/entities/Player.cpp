/* Created by kblaszki */

#include "Player.hpp"

Player::Player(KeyboardManagerI& keyboardManager)
    : leftKeyRegistration{keyboardManager.registerKeyHandler(
          sf::Keyboard::Key::Left,
          [this](const KeyStatus status, const sf::Event::KeyPressed&) { moveLeft = KeyStatus::Pressed == status; })}
    , rightKeyRegistration{keyboardManager.registerKeyHandler(
          sf::Keyboard::Key::Right,
          [this](const KeyStatus status, const sf::Event::KeyPressed&) { moveRight = KeyStatus::Pressed == status; })}
    , downKeyRegistration{keyboardManager.registerKeyHandler(
          sf::Keyboard::Key::Down,
          [this](const KeyStatus status, const sf::Event::KeyPressed&) { moveDown = KeyStatus::Pressed == status; })}
    , upKeyRegistration{keyboardManager.registerKeyHandler(
          sf::Keyboard::Key::Up,
          [this](const KeyStatus status, const sf::Event::KeyPressed&) { moveUp = KeyStatus::Pressed == status; })}
{
    shape.setPointCount(4);

    shape.setPoint(0, sf::Vector2f(300, 200));  // Top-left
    shape.setPoint(1, sf::Vector2f(800, 200));  // Top-right
    shape.setPoint(2, sf::Vector2f(1000, 400)); // Bottom-right
    shape.setPoint(3, sf::Vector2f(100, 400));  // Bottom-left

    shape.setFillColor(sf::Color::Green);
}

void Player::update(float dt)
{
    if(moveLeft)
    {
        shape.move({-SPEED_PX_PER_SEC * dt, 0.f});
    }
    if(moveRight)
    {
        shape.move({SPEED_PX_PER_SEC * dt, 0.f});
    }
    if(moveDown)
    {
        shape.move({0.f, SPEED_PX_PER_SEC * dt});
    }
    if(moveUp)
    {
        shape.move({0.f, -SPEED_PX_PER_SEC * dt});
    }
}

void Player::draw(DrawerI& drawer) const
{
    drawer.draw(shape);
}
