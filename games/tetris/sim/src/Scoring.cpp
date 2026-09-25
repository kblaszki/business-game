#include <cmath>
#include <tetris/sim/Scoring.hpp>

namespace sgl::tetris
{

std::uint32_t lineClearPoints(std::uint8_t cleared, std::uint32_t level)
{
    std::uint32_t base = 0;
    switch(cleared)
    {
        case 1:
            base = 100;
            break;
        case 2:
            base = 300;
            break;
        case 3:
            base = 500;
            break;
        case 4:
            base = 800;
            break;
        default:
            return 0;
    }
    return base * level;
}

sgl::Seconds gravityInterval(std::uint32_t level)
{
    if(level < 1)
    {
        level = 1;
    }
    const double n = static_cast<double>(level - 1);
    const double seconds = std::pow(0.8 - n * 0.007, n);
    return sgl::Seconds{static_cast<float>(seconds)};
}

std::uint32_t levelForLines(std::uint32_t lines)
{
    return 1u + lines / 10u;
}

void applyLineClear(Score& score, std::uint8_t cleared)
{
    if(cleared == 0)
    {
        return;
    }
    score.points += lineClearPoints(cleared, score.level);
    score.lines += cleared;
    score.level = levelForLines(score.lines);
}

void applySoftDrop(Score& score, int rows)
{
    if(rows > 0)
    {
        score.points += static_cast<std::uint32_t>(rows);
    }
}

void applyHardDrop(Score& score, int rows)
{
    if(rows > 0)
    {
        score.points += static_cast<std::uint32_t>(rows) * 2u;
    }
}

} // namespace sgl::tetris
