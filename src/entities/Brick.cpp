/* Created by kblaszki */

#include "Brick.hpp"

Brick::Brick(sf::Vector2f position, sf::Color color)
{
    shape.setPosition(position);
    shape.setSize({WIDTH, HEIGHT});
    shape.setFillColor(color);
}

void Brick::update(float /*dt*/) {}

void Brick::draw(DrawerI& drawer) const
{
    if(not destroyed)
    {
        drawer.draw(shape);
    }
}

sf::FloatRect Brick::getBounds() const
{
    return shape.getGlobalBounds();
}

bool Brick::isDestroyed() const
{
    return destroyed;
}

void Brick::destroy()
{
    destroyed = true;
}
