/* Created by kblaszki */
#pragma once

#include "ScreenI.hpp"
#include "ScreenUpdaterI.hpp"

#include <entities/Ball.hpp>
#include <entities/Brick.hpp>
#include <entities/Paddle.hpp>
#include <managers/EventManagers.hpp>
#include <window/ScreenRendererI.hpp>

#include <optional>
#include <resources/ResourceManager.hpp>
#include <vector>

class GameScreen : public ScreenI
{
public:
    static constexpr float ARENA_WIDTH = 1280.f;
    static constexpr float ARENA_HEIGHT = 720.f;
    static constexpr int INITIAL_LIVES = 3;

    GameScreen(EventManagers& eventManagers,
               ScreenRendererI& screenRenderer,
               ScreenUpdaterI& screenUpdater,
               ResourceManager& resources);

    void update(float dt) override;
    void display() override;

    [[nodiscard]] int lives() const;
    [[nodiscard]] std::size_t remainingBricks() const;
    [[nodiscard]] std::optional<sf::FloatRect> firstAliveBrickBounds() const;
    [[nodiscard]] Ball& ball();
    [[nodiscard]] const Paddle& paddle() const;

private:
    void buildBricks();
    void updateHud();
    void handleWallCollisions();
    void handlePaddleCollision();
    void handleBrickCollisions();
    void loseLife();
    void returnToMenu();

    EventManagers& eventManagers;
    ScreenRendererI& screenRenderer;
    ScreenUpdaterI& screenUpdater;
    ResourceManager& resources;

    Paddle gamePaddle;
    Ball gameBall;
    std::vector<Brick> bricks;
    int livesRemaining{INITIAL_LIVES};
    bool transitioning{false};
    sf::Text livesText;
};
