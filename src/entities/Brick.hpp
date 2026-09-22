/* Created by kblaszki */
#pragma once

#include "EntityI.hpp"

#include <window/DrawerI.hpp>

#include <SFML/Graphics.hpp>

class Brick : public EntityI
{
public:
    static constexpr float WIDTH = 80.f;
    static constexpr float HEIGHT = 30.f;

    Brick(sf::Vector2f position, sf::Color color);

    void update(float dt) override;
    void draw(DrawerI& drawer) const override;

    [[nodiscard]] sf::FloatRect getBounds() const;
    [[nodiscard]] bool isDestroyed() const;
    void destroy();

private:
    sf::RectangleShape shape{};
    bool destroyed{false};
};
