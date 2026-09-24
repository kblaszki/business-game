#include "Brick.hpp"

Brick::Brick(sf::Vector2f position, sf::Color tint, const sf::Texture& texture)
    : sprite{texture}
{
    sprite.setPosition(position);
    sprite.setColor(tint);
}

void Brick::draw(DrawerI& drawer) const
{
    if(live)
    {
        drawer.draw(sprite);
    }
}

void Brick::kill()
{
    live = false;
}

bool Brick::alive() const
{
    return live;
}

sf::Vector2f Brick::position() const
{
    return sprite.getPosition();
}

sf::Vector2f Brick::size() const
{
    return sprite.getLocalBounds().size;
}
