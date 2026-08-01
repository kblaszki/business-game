/* Created by kblaszki */
#pragma once

#include "EntityI.hpp"

#include <managers/KeyboardManagerI.hpp>
#include <window/DrawerI.hpp>

#include <SFML/Graphics.hpp>

class Paddle : public EntityI
{
public:
    static constexpr float SPEED_PX_PER_SEC = 600.f;

    Paddle(KeyboardManagerI& keyboardManager);

    void update(float dt) override;
    void draw(DrawerI& drawer) const override;

    [[nodiscard]] sf::Vector2f getPosition() const;

private:
    sf::RectangleShape shape{};
    KeyboardManagerI::KeyUnRegisterer leftKeyRegistration;
    KeyboardManagerI::KeyUnRegisterer rightKeyRegistration;
    bool moveLeft = false;
    bool moveRight = false;
};
