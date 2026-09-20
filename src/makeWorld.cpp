/* Created by kblaszki */

#include "makeWorld.hpp"

#include <entities/Ball.hpp>
#include <entities/Brick.hpp>
#include <entities/Paddle.hpp>

#include <SFML/Graphics/Color.hpp>

#include <Game.hpp>
#include <memory>

namespace
{
constexpr int brickColumns = 10;
constexpr int brickRows = 5;
constexpr float brickGap = 4.f;
constexpr float brickTop = 60.f;

sf::Color brickColorForRow(int row)
{
    static constexpr sf::Color colors[] = {
        sf::Color{220, 80, 80},
        sf::Color{220, 160, 60},
        sf::Color{220, 220, 80},
        sf::Color{80, 180, 80},
        sf::Color{80, 140, 220},
    };
    return colors[row % 5];
}
} // namespace

World makeWorld(const LevelDescriptor&)
{
    World world;
    auto paddle = std::make_unique<Paddle>();
    auto ball = std::make_unique<Ball>();
    ball->resetAbove(*paddle);
    world.spawn(std::move(paddle));
    world.spawn(std::move(ball));

    const float totalWidth = brickColumns * Brick::WIDTH + (brickColumns - 1) * brickGap;
    const float startX = (static_cast<float>(Game::DESIGN_SIZE.x) - totalWidth) / 2.f;
    for(int row = 0; row < brickRows; ++row)
    {
        for(int col = 0; col < brickColumns; ++col)
        {
            const float x = startX + static_cast<float>(col) * (Brick::WIDTH + brickGap);
            const float y = brickTop + static_cast<float>(row) * (Brick::HEIGHT + brickGap);
            world.spawn(std::make_unique<Brick>(sf::Vector2f{x, y}, brickColorForRow(row)));
        }
    }

    return world;
}
