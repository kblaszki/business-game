/* Created by kblaszki */
#pragma once

#include <GameObject.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class Paddle;

class Ball : public GameObject
{
public:
    static constexpr float RADIUS = 8.f;
    static constexpr float DEFAULT_SPEED = 420.f;

    Ball();

    void fixedUpdate(sf::Time tick) override;
    void draw(sf::RenderTarget& target) const override;
    sf::Vector2f position() const override;
    sf::FloatRect bounds() const override;

    sf::Vector2f velocity() const;
    void setPosition(sf::Vector2f position);
    void setVelocity(sf::Vector2f velocity);
    void resetAbove(const Paddle& paddle);
    bool lost() const;
    void clearLost();

private:
    void resolveWalls();

    sf::CircleShape m_shape;
    sf::Vector2f m_velocity{DEFAULT_SPEED * 0.6f, -DEFAULT_SPEED};
    bool m_lost{false};
};
