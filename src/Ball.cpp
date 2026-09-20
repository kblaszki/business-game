/* Created by kblaszki */

#include "Ball.hpp"

#include "Paddle.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

#include <Game.hpp>
#include <RectCollision.hpp>

Ball::Ball() = default;

void Ball::draw(IDrawer& drawer) const
{
    sf::CircleShape shape{RADIUS};
    shape.setOrigin({RADIUS, RADIUS});
    shape.setPosition(m_position);
    shape.setFillColor(sf::Color::White);
    drawer.draw(shape);
}

void Ball::fixedUpdate(sf::Time tick)
{
    if(m_lost)
    {
        return;
    }

    constexpr float secondsPerTick = 1.f / 60.f;
    m_position += m_velocity * secondsPerTick * (tick / sf::seconds(secondsPerTick));
    resolveWalls();
}

sf::Vector2f Ball::position() const
{
    return m_position;
}

sf::FloatRect Ball::bounds() const
{
    return {{m_position.x - RADIUS, m_position.y - RADIUS}, {RADIUS * 2.f, RADIUS * 2.f}};
}

sf::Vector2f Ball::velocity() const
{
    return m_velocity;
}

void Ball::setPosition(sf::Vector2f position)
{
    m_position = position;
}

void Ball::setVelocity(sf::Vector2f velocity)
{
    m_velocity = velocity;
}

void Ball::resetAbove(const Paddle& paddle)
{
    setPosition({paddle.position().x + paddle.size().x / 2.f, paddle.position().y - RADIUS - 2.f});
    setVelocity({DEFAULT_SPEED * 0.6f, -DEFAULT_SPEED});
    m_lost = false;
}

bool Ball::lost() const
{
    return m_lost;
}

void Ball::clearLost()
{
    m_lost = false;
}

void Ball::resolveWalls()
{
    const sf::FloatRect ballBounds = bounds();
    sf::Vector2f pos = position();
    const float arenaWidth = static_cast<float>(Game::DESIGN_SIZE.x);
    const float arenaHeight = static_cast<float>(Game::DESIGN_SIZE.y);
    bool changed = false;

    if(ballBounds.position.x <= 0.f)
    {
        bounceBallOffVerticalWall(m_velocity);
        pos.x = RADIUS;
        changed = true;
    }
    else if(ballBounds.position.x + ballBounds.size.x >= arenaWidth)
    {
        bounceBallOffVerticalWall(m_velocity);
        pos.x = arenaWidth - RADIUS;
        changed = true;
    }

    if(ballBounds.position.y <= 0.f)
    {
        bounceBallOffHorizontalWall(m_velocity);
        pos.y = RADIUS;
        changed = true;
    }
    else if(ballBounds.position.y + ballBounds.size.y >= arenaHeight)
    {
        m_lost = true;
        return;
    }

    if(changed)
    {
        setPosition(pos);
    }
}
