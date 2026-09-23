#pragma once

#include <window/DrawerI.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class GameObject
{
public:
    GameObject(sf::Vector2f position, sf::Vector2f velocity);

    void fixedUpdate(sf::Time tick);
    void draw(DrawerI& drawer) const;
    [[nodiscard]] sf::Vector2f position() const;

private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
};
