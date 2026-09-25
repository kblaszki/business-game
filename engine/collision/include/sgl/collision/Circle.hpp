#pragma once

#include <sgl/core/Rect.hpp>
#include <sgl/core/Vec2.hpp>

namespace sgl
{

struct Circle
{
    Vec2f center{};
    float radius{};
};

using Aabb = Rect<float>;

} // namespace sgl
