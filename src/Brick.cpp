/* Created by kblaszki */

#include "Brick.hpp"

Brick::Brick(sf::Vector2f position, sf::Color color)
    : m_shape({WIDTH, HEIGHT})
{
    m_shape.setPosition(position);
    m_shape.setFillColor(color);
}

void Brick::fixedUpdate(sf::Time)
{
}

void Brick::draw(sf::RenderTarget& target) const
{
    if(m_destroyed)
    {
        return;
    }

    target.draw(m_shape);
}

sf::Vector2f Brick::position() const
{
    return m_shape.getPosition();
}

sf::FloatRect Brick::bounds() const
{
    return m_shape.getGlobalBounds();
}

bool Brick::alive() const
{
    return !m_destroyed;
}

void Brick::destroy()
{
    m_destroyed = true;
}
