#pragma once

#include <arkanoid/sim/State.hpp>
#include <cstddef>
#include <variant>

namespace sgl::arkanoid
{

struct BrickDestroyed
{
    std::size_t index{};
};

struct LifeLost
{
};

struct StageCleared
{
    StageId stage{};
};

struct GameOver
{
};

struct PowerUpCaught
{
    PowerUpKind kind{};
};

struct PaddleHit
{
};

struct WallHit
{
};

struct BallLaunched
{
};

struct BallLost
{
};

using SimEvent = std::variant<BrickDestroyed,
                              LifeLost,
                              StageCleared,
                              GameOver,
                              PowerUpCaught,
                              PaddleHit,
                              WallHit,
                              BallLaunched,
                              BallLost>;

} // namespace sgl::arkanoid
