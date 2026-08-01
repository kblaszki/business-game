/* Created by kblaszki */
#pragma once

#include "GameWindowManagerI.hpp"

#include <window/WindowViewI.hpp>

#include <SFML/System/Vector2.hpp>

class GameWindowManager : public GameWindowManagerI
{
public:
    static constexpr sf::Vector2f DESIGN_SIZE{1280.f, 720.f};

    explicit GameWindowManager(WindowViewI& windowView);

    void handleEvent(const sf::Event& event) override;

private:
    void applyLetterbox(sf::Vector2u windowSize);

    WindowViewI& windowView;
};
