#pragma once

#include <cmath>
#include <eng/collision/Circle.hpp>
#include <eng/collision/Contact.hpp>
#include <eng/collision/Hit.hpp>
#include <eng/core/Vec2.hpp>
#include <optional>

namespace eng::detail
{

[[nodiscard]] inline bool nearlyZero(float v, float eps = 1e-5f) noexcept
{
    return std::fabs(v) <= eps;
}

} // namespace eng::detail

namespace eng
{

[[nodiscard]] std::optional<Contact> intersect(const Circle& circle, const Aabb& box) noexcept;

[[nodiscard]] std::optional<Hit> sweep(const Circle& circle, Vec2f delta, const Aabb& box) noexcept;

[[nodiscard]] Vec2f reflect(Vec2f velocity, Vec2f normal) noexcept;

} // namespace eng
