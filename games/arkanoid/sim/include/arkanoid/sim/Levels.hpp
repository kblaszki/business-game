#pragma once

#include <arkanoid/sim/State.hpp>
#include <eng/core/Color.hpp>

#include <cstdint>
#include <span>

namespace arkanoid
{

struct LevelData
{
    StageId id{};
    std::uint32_t columns{};
    std::uint32_t rows{};
    std::span<const eng::Color> rowColors{};
    std::span<const std::uint8_t> mask{};
};

[[nodiscard]] const LevelData& level(StageId id);

[[nodiscard]] State makeState(StageId id, std::uint32_t score = 0, std::uint32_t lives = startingLives);

} // namespace arkanoid
