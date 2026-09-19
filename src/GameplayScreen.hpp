/* Created by kblaszki */
#pragma once

#include <IScreen.hpp>
#include <LevelId.hpp>
#include <World.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

class Ball;
class Paddle;
class ScreenStack;

class GameplayScreen : public IScreen
{
public:
    static constexpr int INITIAL_LIVES = 3;

    explicit GameplayScreen(ScreenStack& stack, LevelId levelId);

    bool handleEvent(const sf::Event& event) override;
    bool handleAction(Action action) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
    bool blocksUpdate() const override;
    bool blocksDraw() const override;

    [[nodiscard]] std::uint32_t tickCount() const;
    [[nodiscard]] std::uint32_t drawCount() const;
    [[nodiscard]] std::uint32_t handleActionCount() const;
    [[nodiscard]] int lives() const;
    [[nodiscard]] std::size_t remainingBricks() const;
    [[nodiscard]] sf::Vector2f paddlePosition() const;
    [[nodiscard]] sf::Vector2f ballPosition() const;
    void destroyAllBricksForTest();
    void loseLifeForTest();
    void dropBallForTest();

private:
    void bindObjects();
    void handlePaddleCollision();
    void handleBrickCollisions();
    void loseLife();
    void returnToMenu();
    void drawLives(sf::RenderTarget& target) const;

    ScreenStack& m_stack;
    World m_world;
    Paddle* m_paddle{nullptr};
    Ball* m_ball{nullptr};
    int m_lives{INITIAL_LIVES};
    bool m_ending{false};
    std::uint32_t m_tickCount{0};
    std::uint32_t m_drawCount{0};
    std::uint32_t m_handleActionCount{0};
};
