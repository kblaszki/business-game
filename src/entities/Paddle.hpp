/* Created by kblaszki */
#pragma once

#include "CollidableI.hpp"

class Paddle : public CollidableI
{
public:
    static constexpr float SPEED_PX_PER_SEC = 600.f;
    static constexpr float WIDTH = 100.f;
    static constexpr float HEIGHT = 20.f;

    Paddle();

    void setMoveLeft(bool moveLeft);
    void setMoveRight(bool moveRight);

    void fixedUpdate(sf::Time tick) override;
    void draw(DrawerI& drawer) const override;
    sf::Vector2f position() const override;
    sf::FloatRect bounds() const override;
    sf::Vector2f size() const;

private:
    void clampToArena();

    sf::Vector2f m_position{};
    bool m_moveLeft{false};
    bool m_moveRight{false};
};
