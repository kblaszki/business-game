/* Created by kblaszki */

#include "GameObject.hpp"

#include <SFML/Graphics/Color.hpp>

#include <Game.hpp>

namespace
{
float wrapAxis(float value, float extent)
{
    while(value >= extent)
    {
        value -= extent;
    }
    while(value < 0.f)
    {
        value += extent;
    }
    return value;
}
} // namespace

GameObject::GameObject(sf::Vector2f position, sf::Vector2f velocity)
    : m_position(position)
    , m_velocity(velocity)
    , m_shape({40.f, 40.f})
{
    m_shape.setFillColor(sf::Color::Yellow);
    m_shape.setPosition(m_position);
}

void GameObject::fixedUpdate(sf::Time tick)
{
    constexpr float secondsPerTick = 1.f / 60.f;
    m_position += m_velocity * secondsPerTick * (tick / sf::seconds(secondsPerTick));
    m_position.x = wrapAxis(m_position.x, static_cast<float>(Game::DESIGN_SIZE.x));
    m_position.y = wrapAxis(m_position.y, static_cast<float>(Game::DESIGN_SIZE.y));
    m_shape.setPosition(m_position);
}

void GameObject::draw(sf::RenderTarget& target) const
{
    target.draw(m_shape);
}

sf::Vector2f GameObject::position() const
{
    return m_position;
}
