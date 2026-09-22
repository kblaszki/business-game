/* Created by kblaszki */
#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <algorithm>
#include <cmath>

[[nodiscard]] inline bool intersects(const sf::FloatRect& a, const sf::FloatRect& b)
{
    return a.findIntersection(b).has_value();
}

inline void bounceBallOffVerticalWall(sf::Vector2f& velocity)
{
    velocity.x = -velocity.x;
}

inline void bounceBallOffHorizontalWall(sf::Vector2f& velocity)
{
    velocity.y = -velocity.y;
}

inline void bounceBallOffAabb(sf::Vector2f& velocity, const sf::FloatRect& ballBounds, const sf::FloatRect& block)
{
    const float ballRight = ballBounds.position.x + ballBounds.size.x;
    const float ballBottom = ballBounds.position.y + ballBounds.size.y;
    const float blockRight = block.position.x + block.size.x;
    const float blockBottom = block.position.y + block.size.y;

    const float overlapLeft = ballRight - block.position.x;
    const float overlapRight = blockRight - ballBounds.position.x;
    const float overlapTop = ballBottom - block.position.y;
    const float overlapBottom = blockBottom - ballBounds.position.y;

    const float minOverlapX = std::min(overlapLeft, overlapRight);
    const float minOverlapY = std::min(overlapTop, overlapBottom);

    if(minOverlapX < minOverlapY)
    {
        bounceBallOffVerticalWall(velocity);
    }
    else
    {
        bounceBallOffHorizontalWall(velocity);
    }
}

inline void bounceBallOffPaddle(sf::Vector2f& velocity, const sf::FloatRect& ballBounds, const sf::FloatRect& paddle)
{
    const float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    const float paddleCenterX = paddle.position.x + paddle.size.x / 2.f;
    const float ballCenterX = ballBounds.position.x + ballBounds.size.x / 2.f;
    const float halfWidth = paddle.size.x / 2.f;
    float offset = 0.f;
    if(halfWidth > 0.f)
    {
        offset = std::clamp((ballCenterX - paddleCenterX) / halfWidth, -1.f, 1.f);
    }

    const float angle = offset * 1.04719755f; // ~60 degrees
    velocity.x = std::sin(angle) * speed;
    velocity.y = -std::abs(std::cos(angle) * speed);
}
