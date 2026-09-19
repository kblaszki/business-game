/* Created by kblaszki */
#pragma once

#include <SFML/System/Vector2.hpp>

#include <LevelId.hpp>
#include <span>
#include <utility>

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

inline const LevelDescriptor& levelDescriptor(LevelId id)
{
    static const SpawnSpec sandboxSpawns[]{
        SpawnSpec{{0.f, 340.f}, {240.f, 0.f}},
    };
    static const LevelDescriptor sandboxDescriptor{
        .id = LevelId::Sandbox,
        .spawns = sandboxSpawns,
    };

    switch(id)
    {
        case LevelId::Sandbox:
            return sandboxDescriptor;
    }

    std::unreachable();
}
