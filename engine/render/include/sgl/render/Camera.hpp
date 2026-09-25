#pragma once

#include <sgl/core/Vec2.hpp>

namespace sgl
{

struct Camera
{
    Vec2f offset{};
    float zoom{1.f};

    Vec2f apply(Vec2f screen) const
    {
        return (screen - offset) * zoom;
    }
};

} // namespace sgl
