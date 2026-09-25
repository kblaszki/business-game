#pragma once

#include <cmath>
#include <optional>
#include <sgl/collision/Circle.hpp>
#include <sgl/collision/Contact.hpp>
#include <sgl/collision/Hit.hpp>
#include <sgl/core/Vec2.hpp>

namespace sgl::detail
{

[[nodiscard]] inline bool nearlyZero(float v, float eps = 1e-5f) noexcept
{
    return std::fabs(v) <= eps;
}

} // namespace sgl::detail

namespace sgl
{

[[nodiscard]] std::optional<Contact> intersect(const Circle& circle, const Aabb& box) noexcept;

[[nodiscard]] std::optional<Hit> sweep(const Circle& circle, Vec2f delta, const Aabb& box) noexcept;

[[nodiscard]] Vec2f reflect(Vec2f velocity, Vec2f normal) noexcept;

} // namespace sgl
