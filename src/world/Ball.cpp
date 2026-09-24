#include "Ball.hpp"

#include <time/FixedTimestep.hpp>

Ball::Ball(sf::Vector2f position, const sf::Texture& texture)
    : sprite{texture}
{
    sprite.setPosition(position);
}

void Ball::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}

void Ball::setVelocity(sf::Vector2f velocity)
{
    vel = velocity;
}

void Ball::setStuck(bool stuck)
{
    glued = stuck;
}

void Ball::fixedUpdate(sf::Time tick)
{
    if(glued)
    {
        return;
    }

    auto pos = sprite.getPosition();
    pos += (vel / 60.f) * (tick / FixedTimestep::tick);
    sprite.setPosition(pos);
}

void Ball::draw(DrawerI& drawer) const
{
    drawer.draw(sprite);
}

sf::Vector2f Ball::position() const
{
    return sprite.getPosition();
}

sf::Vector2f Ball::velocity() const
{
    return vel;
}

sf::Vector2f Ball::size() const
{
    return sprite.getLocalBounds().size;
}

float Ball::radius() const
{
    return size().x * 0.5f;
}

sf::Vector2f Ball::center() const
{
    return position() + size() * 0.5f;
}

bool Ball::stuck() const
{
    return glued;
}
