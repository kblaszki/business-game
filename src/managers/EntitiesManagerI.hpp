/* Created by kblaszki */
#pragma once

#include <entities/EntityI.hpp>

#include <memory>

struct EntitiesManagerI
{
    virtual ~EntitiesManagerI() = default;

    template<typename T, typename... Args>
    void createEntity(Args&&... args)
    {
        addEntity(std::make_unique<T>(std::forward<Args>(args)...));
    }

    virtual void addEntity(std::unique_ptr<EntityI>&& entity) = 0;
    virtual void handleCollisions() = 0;
};