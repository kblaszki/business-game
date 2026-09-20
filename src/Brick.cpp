/* Created by kblaszki */

#include "Brick.hpp"

Brick::Brick(sf::Vector2f position, sf::Color color)
    : m_position(position)
    , m_fill(color)
{
}

void Brick::fixedUpdate(sf::Time) {}

sf::Vector2f Brick::position() const
{
    return m_position;
}

sf::FloatRect Brick::bounds() const
{
    return {m_position, {WIDTH, HEIGHT}};
}

bool Brick::alive() const
{
    return !m_destroyed;
}

void Brick::destroy()
{
    m_destroyed = true;
}
