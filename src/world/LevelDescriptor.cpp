#include "LevelDescriptor.hpp"

#include <memory>

namespace
{
const SpawnSpec sandboxSpawns[]{{{0.f, 340.f}, {240.f, 0.f}}};

const LevelDescriptor sandbox{LevelId::Sandbox, sandboxSpawns};
} // namespace

const LevelDescriptor& levelDescriptor(LevelId id)
{
    switch(id)
    {
        case LevelId::Sandbox:
            return sandbox;
    }

    return sandbox;
}

World makeWorld(const LevelDescriptor& descriptor)
{
    World world;
    for(const SpawnSpec& spec: descriptor.spawns)
    {
        world.spawn(std::make_unique<GameObject>(spec.position, spec.velocity));
    }
    return world;
}
