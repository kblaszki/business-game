#pragma once

#include <SFML/Graphics/Color.hpp>

#include <cstdint>
#include <span>
#include <world/LevelId.hpp>
#include <world/World.hpp>

struct BrickRow
{
    sf::Color color{};
};

struct LevelDescriptor
{
    LevelId id{LevelId::Stage1};
    std::uint32_t columns{10};
    std::uint32_t rows{6};
    std::span<const BrickRow> palette{};
};

[[nodiscard]] const LevelDescriptor& levelDescriptor(LevelId id);
[[nodiscard]] World makeWorld(const LevelDescriptor& descriptor);
