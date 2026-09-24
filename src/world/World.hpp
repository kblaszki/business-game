#pragma once

#include <window/DrawerI.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <cstdint>
#include <vector>
#include <world/Ball.hpp>
#include <world/BreakoutArt.hpp>
#include <world/Brick.hpp>
#include <world/Paddle.hpp>

class World
{
public:
    World();

    void setPaddleSpeed(float pxPerSec);
    void launch();
    void placeBall(sf::Vector2f position, sf::Vector2f velocity, bool stuck);
    void addBrick(sf::Vector2f position, sf::Color tint);
    void fixedUpdate(sf::Time tick);
    void draw(DrawerI& drawer) const;

    [[nodiscard]] std::uint32_t tickCount() const;
    [[nodiscard]] std::uint32_t score() const;
    [[nodiscard]] std::uint32_t lives() const;
    [[nodiscard]] bool won() const;
    [[nodiscard]] bool lost() const;
    [[nodiscard]] const Paddle& paddle() const;
    [[nodiscard]] const Ball& ball() const;
    [[nodiscard]] std::size_t brickCount() const;
    [[nodiscard]] const Brick& brickAt(std::size_t index) const;

private:
    void stickBallToPaddle();
    void bounceWalls();
    void bouncePaddle();
    void bounceBricks();
    void missCheck();

    BreakoutArt art;
    Paddle bat;
    Ball sphere;
    std::vector<Brick> bricks;
    std::uint32_t ticks{0};
    std::uint32_t points{0};
    std::uint32_t remaining{3};
};
