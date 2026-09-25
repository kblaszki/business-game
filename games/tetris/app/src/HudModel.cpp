#include <string>
#include <tetris/app/HudModel.hpp>

namespace sgl::tetris
{

HudModel makeHud(const Score& score)
{
    return HudModel{
        .score = "Score " + std::to_string(score.points),
        .lines = "Lines " + std::to_string(score.lines),
        .level = "Level " + std::to_string(score.level),
    };
}

} // namespace sgl::tetris
