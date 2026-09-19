/* Created by kblaszki */
#pragma once

#include <GameObject.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Paddle : public GameObject
{
public:
    static constexpr float SPEED_PX_PER_SEC = 600.f;
    static constexpr float WIDTH = 100.f;
    static constexpr float HEIGHT = 20.f;

    Paddle();

    void setMoveLeft(bool moveLeft);
    void setMoveRight(bool moveRight);

    void fixedUpdate(sf::Time tick) override;
    void draw(sf::RenderTarget& target) const override;
    sf::Vector2f position() const override;
    sf::FloatRect bounds() const override;
    sf::Vector2f size() const;

private:
    void clampToArena();

    sf::RectangleShape m_shape;
    bool m_moveLeft{false};
    bool m_moveRight{false};
};
