#pragma once

#include <arkanoid/sim/State.hpp>
#include <optional>
#include <string>

namespace sgl::arkanoid
{

struct HudModel
{
    std::string score;
    std::string lives;
    std::string effect;
    std::optional<std::string> banner;
    std::optional<std::string> hint;
};

[[nodiscard]] HudModel makeHud(const State& state);

} // namespace sgl::arkanoid
