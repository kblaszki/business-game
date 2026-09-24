#pragma once

#include <eng/core/Vec2.hpp>

namespace eng
{

template<class T>
struct Rect
{
    Vec2<T> pos{};
    Vec2<T> size{};

    constexpr bool contains(const Vec2<T>& point) const noexcept
    {
        return point.x >= pos.x && point.x < pos.x + size.x && point.y >= pos.y && point.y < pos.y + size.y;
    }

    constexpr bool intersects(const Rect& other) const noexcept
    {
        const T ax0 = pos.x;
        const T ay0 = pos.y;
        const T ax1 = pos.x + size.x;
        const T ay1 = pos.y + size.y;
        const T bx0 = other.pos.x;
        const T by0 = other.pos.y;
        const T bx1 = other.pos.x + other.size.x;
        const T by1 = other.pos.y + other.size.y;
        return ax0 < bx1 && bx0 < ax1 && ay0 < by1 && by0 < ay1;
    }
};

} // namespace eng
