#pragma once

#include <sgl/core/Time.hpp>
#include <variant>
#include <vector>

namespace sgl::arkanoid
{

struct Wide
{
    sgl::Seconds remaining{};
};

struct Slow
{
    sgl::Seconds remaining{};
};

using TimedEffect = std::variant<Wide, Slow>;

struct Effects
{
    std::vector<TimedEffect> active{};
};

[[nodiscard]] float paddleWidth(const Effects& effects);

[[nodiscard]] float ballSpeedMultiplier(const Effects& effects);

} // namespace sgl::arkanoid
