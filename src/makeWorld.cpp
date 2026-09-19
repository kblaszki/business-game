/* Created by kblaszki */

#include "makeWorld.hpp"

#include <GameObject.hpp>
#include <memory>

World makeWorld(const LevelDescriptor& descriptor)
{
    World world;
    for(const SpawnSpec& spec: descriptor.spawns)
    {
        world.spawn(std::make_unique<GameObject>(spec.position, spec.velocity));
    }
    return world;
}
