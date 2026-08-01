/* Created by kblaszki */

#include "Paddle.hpp"

#include <algorithm>

Paddle::Paddle(KeyboardManagerI& keyboardManager)
    : leftKeyRegistration{keyboardManager.registerKeyHandler(
          sf::Keyboard::Key::Left,
          [this](const KeyStatus status, const sf::Event::KeyPressed&) { moveLeft = KeyStatus::Pressed == status; })}
    , rightKeyRegistration{keyboardManager.registerKeyHandler(
          sf::Keyboard::Key::Right,
          [this](const KeyStatus status, const sf::Event::KeyPressed&) { moveRight = KeyStatus::Pressed == status; })}
{
    shape.setPosition({(ARENA_WIDTH - WIDTH) / 2.f, 680.f});
    shape.setSize(sf::Vector2f(WIDTH, HEIGHT));
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
    clampToArena();
}

void Paddle::draw(DrawerI& drawer) const
{
    drawer.draw(shape);
}

sf::Vector2f Paddle::getPosition() const
{
    return shape.getPosition();
}

sf::Vector2f Paddle::getSize() const
{
    return shape.getSize();
}

sf::FloatRect Paddle::getBounds() const
{
    return shape.getGlobalBounds();
}

void Paddle::clampToArena()
{
    sf::Vector2f position = shape.getPosition();
    position.x = std::clamp(position.x, 0.f, ARENA_WIDTH - WIDTH);
    shape.setPosition(position);
}
