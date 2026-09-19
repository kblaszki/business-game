/* Created by kblaszki */
#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class GameObject
{
public:
    GameObject(sf::Vector2f position, sf::Vector2f velocity);

    void fixedUpdate(sf::Time tick);
    void draw(sf::RenderTarget& target) const;

    sf::Vector2f position() const;

private:
    sf::Vector2f m_position{};
    sf::Vector2f m_velocity{};
    sf::RectangleShape m_shape;
};
