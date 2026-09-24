#pragma once

#include <window/DrawerI.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class Ball
{
public:
    Ball(sf::Vector2f position, const sf::Texture& texture);

    void setPosition(sf::Vector2f position);
    void setVelocity(sf::Vector2f velocity);
    void setStuck(bool stuck);
    void fixedUpdate(sf::Time tick);
    void draw(DrawerI& drawer) const;
    [[nodiscard]] sf::Vector2f position() const;
    [[nodiscard]] sf::Vector2f velocity() const;
    [[nodiscard]] sf::Vector2f size() const;
    [[nodiscard]] float radius() const;
    [[nodiscard]] sf::Vector2f center() const;
    [[nodiscard]] bool stuck() const;

private:
    sf::Sprite sprite;
    sf::Vector2f vel{};
    bool glued{true};
};
