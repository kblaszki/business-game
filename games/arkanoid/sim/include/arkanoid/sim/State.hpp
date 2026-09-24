#pragma once

#include <arkanoid/sim/Tuning.hpp>
#include <eng/core/Color.hpp>
#include <eng/core/Rect.hpp>
#include <eng/core/Time.hpp>
#include <eng/core/Vec2.hpp>

#include <cstdint>
#include <optional>
#include <vector>

namespace arkanoid
{

enum class BallMode
{
    Stuck,
    Live
};

struct Ball
{
    eng::Vec2f pos{};
    eng::Vec2f vel{};
    BallMode mode{BallMode::Stuck};
};

struct Paddle
{
    float x{};
    float width{paddleWidth};
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
    eng::Rect<float> box{};
    eng::Color tint{};
    bool alive{true};
    std::optional<PowerUpKind> drop{};
};

struct Capsule
{
    eng::Vec2f pos{};
    PowerUpKind kind{};
};

struct Effects
{
    std::optional<PowerUpKind> timed{};
    eng::Seconds remaining{};
    bool slowActive{false};
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

} // namespace arkanoid
