/* Created by kblaszki */

#include "Paddle.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <Game.hpp>
#include <algorithm>

Paddle::Paddle()
    : m_position({(static_cast<float>(Game::DESIGN_SIZE.x) - WIDTH) / 2.f, 680.f})
{
}

void Paddle::setMoveLeft(bool moveLeft)
{
    m_moveLeft = moveLeft;
}

void Paddle::setMoveRight(bool moveRight)
{
    m_moveRight = moveRight;
}

void Paddle::fixedUpdate(sf::Time tick)
{
    constexpr float secondsPerTick = 1.f / 60.f;
    const float step = SPEED_PX_PER_SEC * secondsPerTick * (tick / sf::seconds(secondsPerTick));
    if(m_moveLeft)
    {
        m_position.x -= step;
    }
    if(m_moveRight)
    {
        m_position.x += step;
    }
    clampToArena();
}

sf::Vector2f Paddle::position() const
{
    return m_position;
}

sf::FloatRect Paddle::bounds() const
{
    return {m_position, {WIDTH, HEIGHT}};
}

sf::Vector2f Paddle::size() const
{
    return {WIDTH, HEIGHT};
}

void Paddle::draw(DrawerI& drawer) const
{
    sf::RectangleShape shape{{WIDTH, HEIGHT}};
    shape.setPosition(m_position);
    shape.setFillColor(sf::Color::Green);
    drawer.draw(shape);
}

void Paddle::clampToArena()
{
    m_position.x = std::clamp(m_position.x, 0.f, static_cast<float>(Game::DESIGN_SIZE.x) - WIDTH);
}
