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

void World::draw(sf::RenderTarget& target) const
{
    for(const auto& object: m_objects)
    {
        object->draw(target);
    }
}

std::size_t World::objectCount() const
{
    return m_objects.size();
}

sf::Vector2f World::dummyPosition() const
{
    assert(!m_objects.empty());
    return m_objects.front()->position();
}
