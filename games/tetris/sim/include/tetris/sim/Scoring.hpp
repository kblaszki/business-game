#pragma once

#include <cstdint>
#include <sgl/core/Time.hpp>

namespace sgl::tetris
{

struct Score
{
    std::uint32_t points{};
    std::uint32_t lines{};
    std::uint32_t level{1};
};

[[nodiscard]] std::uint32_t lineClearPoints(std::uint8_t cleared, std::uint32_t level);
[[nodiscard]] sgl::Seconds gravityInterval(std::uint32_t level);
[[nodiscard]] std::uint32_t levelForLines(std::uint32_t lines);

void applyLineClear(Score& score, std::uint8_t cleared);
void applySoftDrop(Score& score, int rows);
void applyHardDrop(Score& score, int rows);

} // namespace sgl::tetris
