/* Created by kblaszki */

#include "Ball.hpp"

Ball::Ball()
    : shape{RADIUS}
{
    shape.setFillColor(sf::Color::White);
    shape.setOrigin({RADIUS, RADIUS});
    setPosition({640.f, 640.f});
}

void Ball::update(float dt)
{
    shape.move(velocity * dt);
}

void Ball::draw(DrawerI& drawer) const
{
    drawer.draw(shape);
}

sf::FloatRect Ball::getBounds() const
{
    return shape.getGlobalBounds();
}

sf::Vector2f Ball::getPosition() const
{
    return shape.getPosition();
}

sf::Vector2f Ball::getVelocity() const
{
    return velocity;
}

void Ball::setPosition(sf::Vector2f position)
{
    shape.setPosition(position);
}

void Ball::setVelocity(sf::Vector2f newVelocity)
{
    velocity = newVelocity;
}

void Ball::resetAbove(sf::Vector2f paddleTopLeft, sf::Vector2f paddleSize)
{
    setPosition({paddleTopLeft.x + paddleSize.x / 2.f, paddleTopLeft.y - RADIUS - 2.f});
    setVelocity({DEFAULT_SPEED * 0.6f, -DEFAULT_SPEED});
}
