#pragma once

#include <string>
#include <tetris/sim/Scoring.hpp>

namespace sgl::tetris
{

struct HudModel
{
    std::string score;
    std::string lines;
    std::string level;
};

[[nodiscard]] HudModel makeHud(const Score& score);

} // namespace sgl::tetris
