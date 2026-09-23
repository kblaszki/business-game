#include "World.hpp"

#include <stdexcept>

void World::spawn(std::unique_ptr<GameObject> object)
{
    if(!object)
    {
        return;
    }

    if(insideFixedUpdate)
    {
        pendingSpawn.push_back(std::move(object));
        return;
    }

    objects.push_back(std::move(object));
}

void World::fixedUpdate(sf::Time tick)
{
    insideFixedUpdate = true;
    for(auto& object: objects)
    {
        object->fixedUpdate(tick);
    }
    insideFixedUpdate = false;

    for(auto& object: pendingSpawn)
    {
        objects.push_back(std::move(object));
    }
    pendingSpawn.clear();
    ++ticks;
}

void World::draw(DrawerI& drawer) const
{
    for(const auto& object: objects)
    {
        object->draw(drawer);
    }
}

std::uint32_t World::tickCount() const
{
    return ticks;
}

std::size_t World::objectCount() const
{
    return objects.size();
}

const GameObject& World::objectAt(std::size_t index) const
{
    if(index >= objects.size())
    {
        throw std::out_of_range{"World::objectAt"};
    }

    return *objects[index];
}
