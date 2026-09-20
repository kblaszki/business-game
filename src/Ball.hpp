/* Created by kblaszki */
#pragma once

#include <GameObject.hpp>

class Paddle;

class Ball : public GameObject
{
public:
    static constexpr float RADIUS = 8.f;
    static constexpr float DEFAULT_SPEED = 420.f;

    Ball();

    void fixedUpdate(sf::Time tick) override;
    void draw(DrawerI& drawer) const override;
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

    sf::Vector2f m_position{640.f, 640.f};
    sf::Vector2f m_velocity{DEFAULT_SPEED * 0.6f, -DEFAULT_SPEED};
    bool m_lost{false};
};
