#pragma once

#include <cmath>
#include <concepts>
#include <optional>

namespace sgl
{

template<std::floating_point T>
struct Vec3
{
    T x{};
    T y{};
    T z{};

    constexpr Vec3& operator+=(const Vec3& other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    friend constexpr Vec3 operator+(Vec3 a, const Vec3& b) noexcept
    {
        a += b;
        return a;
    }

    friend constexpr Vec3 operator-(Vec3 a, const Vec3& b) noexcept
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
        return a;
    }

    friend constexpr Vec3 operator*(Vec3 v, T scalar) noexcept
    {
        v.x *= scalar;
        v.y *= scalar;
        v.z *= scalar;
        return v;
    }

    friend constexpr Vec3 operator*(T scalar, Vec3 v) noexcept
    {
        return v * scalar;
    }

    constexpr auto operator<=>(const Vec3&) const = default;
    constexpr bool operator==(const Vec3&) const = default;
};

using Vec3f = Vec3<float>;

template<std::floating_point T>
constexpr T dot(const Vec3<T>& a, const Vec3<T>& b) noexcept
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<std::floating_point T>
constexpr T lengthSq(const Vec3<T>& v) noexcept
{
    return dot(v, v);
}

template<std::floating_point T>
constexpr std::optional<Vec3<T>> normalized(const Vec3<T>& v) noexcept
{
    const T lenSq = lengthSq(v);
    if(lenSq == T{})
    {
        return std::nullopt;
    }
    const T invLen = T{1} / std::sqrt(lenSq);
    return Vec3<T>{v.x * invLen, v.y * invLen, v.z * invLen};
}

} // namespace sgl
