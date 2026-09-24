#pragma once

#include <window/DrawerI.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class Paddle
{
public:
    Paddle(sf::Vector2f position, const sf::Texture& texture);

    void setSpeed(float pxPerSec);
    void setDisplayWidth(float width);
    void fixedUpdate(sf::Time tick);
    void draw(DrawerI& drawer) const;
    [[nodiscard]] sf::Vector2f position() const;
    [[nodiscard]] sf::Vector2f size() const;

private:
    sf::Sprite sprite;
    float speed{0.f};
};
