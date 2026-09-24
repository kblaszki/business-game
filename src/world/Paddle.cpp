#include "Paddle.hpp"

#include <Game.hpp>
#include <time/FixedTimestep.hpp>

Paddle::Paddle(sf::Vector2f position, const sf::Texture& texture)
    : sprite{texture}
{
    sprite.setPosition(position);
}

void Paddle::setSpeed(float pxPerSec)
{
    speed = pxPerSec;
}

void Paddle::setDisplayWidth(float width)
{
    const float native = sprite.getLocalBounds().size.x;
    if(native > 0.f)
    {
        sprite.setScale({width / native, sprite.getScale().y});
    }

    auto pos = sprite.getPosition();
    const float maxX = static_cast<float>(Game::DESIGN_SIZE.x) - size().x;
    if(pos.x < 0.f)
    {
        pos.x = 0.f;
    }
    if(pos.x > maxX)
    {
        pos.x = maxX;
    }
    sprite.setPosition(pos);
}

void Paddle::fixedUpdate(sf::Time tick)
{
    auto pos = sprite.getPosition();
    pos.x += (speed / 60.f) * (tick / FixedTimestep::tick);
    const float maxX = static_cast<float>(Game::DESIGN_SIZE.x) - size().x;
    if(pos.x < 0.f)
    {
        pos.x = 0.f;
    }
    if(pos.x > maxX)
    {
        pos.x = maxX;
    }
    sprite.setPosition(pos);
}

void Paddle::draw(DrawerI& drawer) const
{
    drawer.draw(sprite);
}

sf::Vector2f Paddle::position() const
{
    return sprite.getPosition();
}

sf::Vector2f Paddle::size() const
{
    const auto bounds = sprite.getLocalBounds();
    const auto scale = sprite.getScale();
    return {bounds.size.x * scale.x, bounds.size.y * scale.y};
}
