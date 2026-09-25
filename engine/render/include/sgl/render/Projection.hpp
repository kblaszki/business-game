#pragma once

#include <concepts>
#include <sgl/core/Vec2.hpp>
#include <sgl/core/Vec3.hpp>

namespace sgl
{

struct Projected
{
    Vec2f screen;
    float depth;
};

template<class P>
concept Projection = requires(const P& p, Vec3f w) {
    { p.project(w) } -> std::same_as<Projected>;
};

struct Orthographic
{
    Projected project(Vec3f world) const
    {
        return Projected{.screen = Vec2f{world.x, world.y}, .depth = world.z};
    }
};

struct Isometric
{
    float tileW;
    float tileH;

    Projected project(Vec3f world) const
    {
        return Projected{
            .screen =
                Vec2f{
                    (world.x - world.y) * tileW / 2.f,
                    (world.x + world.y) * tileH / 2.f - world.z * tileH,
                },
            .depth = world.x + world.y + world.z * 0.001f,
        };
    }
};

} // namespace sgl
