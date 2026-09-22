/* Created by kblaszki */

#include "World.hpp"

#include <cassert>

void World::spawn(std::unique_ptr<GameObject> object)
{
    m_objects.push_back(std::move(object));
}

void World::fixedUpdate(sf::Time tick)
{
    for(auto& object: m_objects)
    {
        object->fixedUpdate(tick);
    }
}

std::size_t World::objectCount() const
{
    return m_objects.size();
}

GameObject* World::objectAt(std::size_t index)
{
    assert(index < m_objects.size());
    return m_objects[index].get();
}

const GameObject* World::objectAt(std::size_t index) const
{
    assert(index < m_objects.size());
    return m_objects[index].get();
}
