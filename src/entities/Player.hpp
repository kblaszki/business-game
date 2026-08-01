/* Created by kblaszki */
#pragma once

#include "EntityI.hpp"

#include <managers/KeyboardManagerI.hpp>
#include <window/DrawerI.hpp>

#include <SFML/Graphics.hpp>

class Player : public EntityI
{
public:
    static constexpr float SPEED_PX_PER_SEC = 600.f;

    Player(KeyboardManagerI& keyboardManager);

    void update(float dt) override;
    void draw(DrawerI& drawer) const override;

private:
    sf::ConvexShape shape{};
    KeyboardManagerI::KeyUnRegisterer leftKeyRegistration;
    KeyboardManagerI::KeyUnRegisterer rightKeyRegistration;
    KeyboardManagerI::KeyUnRegisterer downKeyRegistration;
    KeyboardManagerI::KeyUnRegisterer upKeyRegistration;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
};
