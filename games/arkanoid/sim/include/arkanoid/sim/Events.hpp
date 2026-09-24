#pragma once

#include <arkanoid/sim/State.hpp>

#include <cstddef>
#include <variant>

namespace arkanoid
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

using SimEvent = std::variant<BrickDestroyed, LifeLost, StageCleared, GameOver, PowerUpCaught>;

} // namespace arkanoid
