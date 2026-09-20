/* Created by kblaszki */

#include "Brick.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

Brick::Brick(sf::Vector2f position, sf::Color color)
    : m_position(position)
    , m_fill(color)
{
}

void Brick::fixedUpdate(sf::Time) {}

void Brick::draw(IDrawer& drawer) const
{
    if(m_destroyed)
    {
        return;
    }

    sf::RectangleShape shape{{WIDTH, HEIGHT}};
    shape.setPosition(m_position);
    shape.setFillColor(m_fill);
    drawer.draw(shape);
}

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
