#pragma once

#include <SFML/System/Vector2.hpp>

#include <span>
#include <world/LevelId.hpp>
#include <world/World.hpp>

struct SpawnSpec
{
    sf::Vector2f position{};
    sf::Vector2f velocity{};
};

struct LevelDescriptor
{
    LevelId id{LevelId::Sandbox};
    std::span<const SpawnSpec> spawns{};
};

[[nodiscard]] const LevelDescriptor& levelDescriptor(LevelId id);
[[nodiscard]] World makeWorld(const LevelDescriptor& descriptor);
