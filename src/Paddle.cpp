/* Created by kblaszki */

#include "Paddle.hpp"

#include <Game.hpp>
#include <SFML/Graphics/Color.hpp>

#include <algorithm>

Paddle::Paddle()
    : m_shape({WIDTH, HEIGHT})
{
    m_shape.setFillColor(sf::Color::Green);
    m_shape.setPosition({(static_cast<float>(Game::DESIGN_SIZE.x) - WIDTH) / 2.f, 680.f});
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
        m_shape.move({-step, 0.f});
    }
    if(m_moveRight)
    {
        m_shape.move({step, 0.f});
    }
    clampToArena();
}

void Paddle::draw(sf::RenderTarget& target) const
{
    target.draw(m_shape);
}

sf::Vector2f Paddle::position() const
{
    return m_shape.getPosition();
}

sf::FloatRect Paddle::bounds() const
{
    return m_shape.getGlobalBounds();
}

sf::Vector2f Paddle::size() const
{
    return m_shape.getSize();
}

void Paddle::clampToArena()
{
    sf::Vector2f pos = m_shape.getPosition();
    pos.x = std::clamp(pos.x, 0.f, static_cast<float>(Game::DESIGN_SIZE.x) - WIDTH);
    m_shape.setPosition(pos);
}
