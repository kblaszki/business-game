#pragma once

#include <arkanoid/sim/State.hpp>
#include <cstdint>
#include <sgl/core/Color.hpp>
#include <span>

namespace sgl::arkanoid
{

struct LevelData
{
    StageId id{};
    std::uint32_t columns{};
    std::uint32_t rows{};
    std::span<const sgl::Color> rowColors{};
    std::span<const std::uint8_t> mask{};
};

[[nodiscard]] const LevelData& level(StageId id);

[[nodiscard]] State makeState(StageId id, std::uint32_t score = 0, std::uint32_t lives = startingLives);

} // namespace sgl::arkanoid
