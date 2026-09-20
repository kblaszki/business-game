/* Created by kblaszki */
#pragma once

#include <SFML/Graphics/Color.hpp>

#include <CollidableI.hpp>

class Brick : public CollidableI
{
public:
    static constexpr float WIDTH = 80.f;
    static constexpr float HEIGHT = 30.f;

    Brick(sf::Vector2f position, sf::Color color);

    void fixedUpdate(sf::Time tick) override;
    void draw(DrawerI& drawer) const override;
    sf::Vector2f position() const override;
    sf::FloatRect bounds() const override;
    bool alive() const override;
    void destroy();

private:
    sf::Vector2f m_position{};
    sf::Color m_fill{};
    bool m_destroyed{false};
};
