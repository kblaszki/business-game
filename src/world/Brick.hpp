#pragma once

#include <window/DrawerI.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

class Brick
{
public:
    Brick(sf::Vector2f position, sf::Color tint, const sf::Texture& texture);

    void draw(DrawerI& drawer) const;
    void kill();
    [[nodiscard]] bool alive() const;
    [[nodiscard]] sf::Vector2f position() const;
    [[nodiscard]] sf::Vector2f size() const;

private:
    sf::Sprite sprite;
    bool live{true};
};
