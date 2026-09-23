#pragma once

#include <window/DrawerI.hpp>

#include <SFML/System/Time.hpp>

#include <cstdint>
#include <memory>
#include <vector>
#include <world/GameObject.hpp>

class World
{
public:
    void spawn(std::unique_ptr<GameObject> object);
    void fixedUpdate(sf::Time tick);
    void draw(DrawerI& drawer) const;
    [[nodiscard]] std::uint32_t tickCount() const;
    [[nodiscard]] std::size_t objectCount() const;
    [[nodiscard]] const GameObject& objectAt(std::size_t index) const;

private:
    std::vector<std::unique_ptr<GameObject>> objects;
    std::vector<std::unique_ptr<GameObject>> pendingSpawn;
    bool insideFixedUpdate{false};
    std::uint32_t ticks{0};
};
