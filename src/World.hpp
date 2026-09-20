/* Created by kblaszki */
#pragma once

#include <SFML/System/Time.hpp>

#include <GameObject.hpp>
#include <IDrawer.hpp>
#include <cstddef>
#include <memory>
#include <vector>

class World
{
public:
    void spawn(std::unique_ptr<GameObject> object);
    void fixedUpdate(sf::Time tick);
    void draw(IDrawer& drawer) const;

    std::size_t objectCount() const;
    GameObject* objectAt(std::size_t index);
    const GameObject* objectAt(std::size_t index) const;

private:
    std::vector<std::unique_ptr<GameObject>> m_objects;
};
