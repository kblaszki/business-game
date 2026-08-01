/* Created by kblaszki */

#include "Paddle.hpp"

Paddle::Paddle(KeyboardManagerI& keyboardManager)
    : leftKeyRegistration{keyboardManager.registerKeyHandler(
          sf::Keyboard::Key::Left,
          [this](const KeyStatus status, const sf::Event::KeyPressed&) { moveLeft = KeyStatus::Pressed == status; })}
    , rightKeyRegistration{keyboardManager.registerKeyHandler(
          sf::Keyboard::Key::Right,
          [this](const KeyStatus status, const sf::Event::KeyPressed&) { moveRight = KeyStatus::Pressed == status; })}
{
    shape.setPosition({400, 500});
    shape.setSize(sf::Vector2f(100, 20));
    shape.setFillColor(sf::Color::Green);
}

void Paddle::update(float dt)
{
    if(moveLeft)
    {
        shape.move({-SPEED_PX_PER_SEC * dt, 0.f});
    }
    if(moveRight)
    {
        shape.move({SPEED_PX_PER_SEC * dt, 0.f});
    }
}

void Paddle::draw(DrawerI& drawer) const
{
    drawer.draw(shape);
}
