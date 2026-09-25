#pragma once

#include <arkanoid/sim/Effects.hpp>
#include <arkanoid/sim/Tuning.hpp>
#include <cstdint>
#include <optional>
#include <sgl/core/Color.hpp>
#include <sgl/core/Rect.hpp>
#include <sgl/core/Vec2.hpp>
#include <vector>

namespace sgl::arkanoid
{

enum class BallMode
{
    Stuck,
    Live
};

struct Ball
{
    sgl::Vec2f pos{};
    sgl::Vec2f vel{};
    BallMode mode{BallMode::Stuck};
};

struct Paddle
{
    float x{};
    float width{paddleNormalWidth};
    float speed{};
};

enum class PowerUpKind
{
    Wide,
    MultiBall,
    Slow,
    ExtraLife
};

struct Brick
{
    sgl::Rect<float> box{};
    sgl::Color tint{};
    bool alive{true};
    std::optional<PowerUpKind> drop{};
};

struct Capsule
{
    sgl::Vec2f pos{};
    PowerUpKind kind{};
};

enum class StageId
{
    Stage1,
    Stage2,
    Stage3
};

struct State
{
    Paddle paddle{};
    std::vector<Ball> balls{};
    std::vector<Brick> bricks{};
    std::vector<Capsule> capsules{};
    Effects effects{};
    std::uint32_t score{};
    std::uint32_t lives{startingLives};
    StageId stage{StageId::Stage1};
    bool cleared{false};
    bool over{false};
};

} // namespace sgl::arkanoid
