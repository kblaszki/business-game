/* Created by kblaszki */
#pragma once

#include "EntityI.hpp"

#include <window/DrawerI.hpp>

#include <SFML/Graphics.hpp>

class Ball : public EntityI
{
public:
    static constexpr float RADIUS = 8.f;
    static constexpr float DEFAULT_SPEED = 420.f;

    Ball();

    void update(float dt) override;
    void draw(DrawerI& drawer) const override;

    [[nodiscard]] sf::FloatRect getBounds() const;
    [[nodiscard]] sf::Vector2f getPosition() const;
    [[nodiscard]] sf::Vector2f getVelocity() const;

    void setPosition(sf::Vector2f position);
    void setVelocity(sf::Vector2f velocity);
    void resetAbove(sf::Vector2f paddleTopLeft, sf::Vector2f paddleSize);

private:
    sf::CircleShape shape;
    sf::Vector2f velocity{DEFAULT_SPEED * 0.6f, -DEFAULT_SPEED};
};
