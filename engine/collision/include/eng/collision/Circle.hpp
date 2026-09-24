#pragma once

#include <eng/core/Rect.hpp>
#include <eng/core/Vec2.hpp>

namespace eng
{

struct Circle
{
    Vec2f center{};
    float radius{};
};

using Aabb = Rect<float>;

} // namespace eng
