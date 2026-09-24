#pragma once

#include <window/DrawerI.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <world/Ball.hpp>
#include <world/BreakoutArt.hpp>
#include <world/Brick.hpp>
#include <world/Paddle.hpp>
#include <world/PowerUp.hpp>

class World
{
public:
    World();

    void setPaddleSpeed(float pxPerSec);
    void launch();
    void placeBall(sf::Vector2f position, sf::Vector2f velocity, bool stuck);
    void addBrick(sf::Vector2f position, sf::Color tint);
    void killBrick(std::size_t index);
    void setScore(std::uint32_t value);
    void setLives(std::uint32_t value);
    void applyPowerUp(PowerUpKind kind);
    void fixedUpdate(sf::Time tick);
    void draw(DrawerI& drawer) const;

    [[nodiscard]] std::uint32_t tickCount() const;
    [[nodiscard]] std::uint32_t score() const;
    [[nodiscard]] std::uint32_t lives() const;
    [[nodiscard]] bool won() const;
    [[nodiscard]] bool lost() const;
    [[nodiscard]] const Paddle& paddle() const;
    [[nodiscard]] const Ball& ball() const;
    [[nodiscard]] std::size_t ballCount() const;
    [[nodiscard]] const Ball& ballAt(std::size_t index) const;
    [[nodiscard]] std::size_t brickCount() const;
    [[nodiscard]] const Brick& brickAt(std::size_t index) const;
    [[nodiscard]] std::size_t powerUpCount() const;
    [[nodiscard]] const PowerUp& powerUpAt(std::size_t index) const;
    [[nodiscard]] std::string activePowerUpName() const;

private:
    void stickBall(Ball& sphere);
    void stickBallToPaddle();
    void bounceWalls(Ball& sphere);
    void bouncePaddle(Ball& sphere);
    void bounceBricks(Ball& sphere);
    void missCheck();
    void spawnDonorCapsule(std::size_t index);
    void updatePowerUps(sf::Time tick);
    void tickTimedEffect();
    void expireTimedEffect();
    void applyMultiBall();

    BreakoutArt art;
    Paddle bat;
    std::vector<Ball> spheres;
    std::vector<Brick> bricks;
    std::vector<PowerUp> capsules;
    std::uint32_t ticks{0};
    std::uint32_t points{0};
    std::uint32_t remaining{3};
    std::uint32_t timedRemaining{0};
    std::optional<PowerUpKind> timedEffect;
    bool slowActive{false};
};
