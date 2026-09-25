#include <algorithm>
#include <cmath>
#include <limits>
#include <sgl/collision/Collision.hpp>

namespace sgl
{
namespace
{

Vec2f outwardNormalForMinPenetration(Vec2f center, const Aabb& box) noexcept
{
    const float left = box.pos.x;
    const float top = box.pos.y;
    const float right = box.pos.x + box.size.x;
    const float bottom = box.pos.y + box.size.y;

    const float distLeft = center.x - left;
    const float distRight = right - center.x;
    const float distTop = center.y - top;
    const float distBottom = bottom - center.y;

    float minDist = distLeft;
    Vec2f normal{-1.f, 0.f};

    if(distRight < minDist)
    {
        minDist = distRight;
        normal = {1.f, 0.f};
    }
    if(distTop < minDist)
    {
        minDist = distTop;
        normal = {0.f, -1.f};
    }
    if(distBottom < minDist)
    {
        normal = {0.f, 1.f};
    }

    return normal;
}

float minPenetrationDepth(Vec2f center, const Aabb& box) noexcept
{
    const float left = box.pos.x;
    const float top = box.pos.y;
    const float right = box.pos.x + box.size.x;
    const float bottom = box.pos.y + box.size.y;

    const float distLeft = center.x - left;
    const float distRight = right - center.x;
    const float distTop = center.y - top;
    const float distBottom = bottom - center.y;

    return std::min({distLeft, distRight, distTop, distBottom});
}

} // namespace

std::optional<Contact> intersect(const Circle& circle, const Aabb& box) noexcept
{
    const float left = box.pos.x;
    const float top = box.pos.y;
    const float right = box.pos.x + box.size.x;
    const float bottom = box.pos.y + box.size.y;

    const float nearestX = std::clamp(circle.center.x, left, right);
    const float nearestY = std::clamp(circle.center.y, top, bottom);
    const float dx = circle.center.x - nearestX;
    const float dy = circle.center.y - nearestY;

    if(detail::nearlyZero(dx) && detail::nearlyZero(dy))
    {
        const float edgeDist = minPenetrationDepth(circle.center, box);
        return Contact{.normal = outwardNormalForMinPenetration(circle.center, box), .depth = edgeDist + circle.radius};
    }

    const float distSq = dx * dx + dy * dy;
    const float radiusSq = circle.radius * circle.radius;
    if(distSq > radiusSq)
    {
        return std::nullopt;
    }

    const float dist = std::sqrt(distSq);
    if(detail::nearlyZero(dist))
    {
        return Contact{.normal = outwardNormalForMinPenetration(circle.center, box), .depth = circle.radius};
    }

    return Contact{.normal = {dx / dist, dy / dist}, .depth = circle.radius - dist};
}

std::optional<Hit> sweep(const Circle& circle, Vec2f delta, const Aabb& box) noexcept
{
    if(const auto contact = intersect(circle, box))
    {
        return Hit{.time = 0.f, .normal = contact->normal};
    }

    const float r = circle.radius;
    const float left = box.pos.x - r;
    const float right = box.pos.x + box.size.x + r;
    const float top = box.pos.y - r;
    const float bottom = box.pos.y + box.size.y + r;
    const Vec2f o = circle.center;

    float tEnter = -std::numeric_limits<float>::infinity();
    float tExit = std::numeric_limits<float>::infinity();
    Vec2f enterNormal{0.f, 0.f};
    bool hasEnterNormal = false;

    auto clipAxis =
        [&](float origin, float dir, float nearPlane, float farPlane, Vec2f nearNormal, Vec2f farNormal) -> bool {
        if(detail::nearlyZero(dir))
        {
            return !(origin < nearPlane || origin > farPlane);
        }

        const float inv = 1.f / dir;
        float t1 = (nearPlane - origin) * inv;
        float t2 = (farPlane - origin) * inv;
        Vec2f n1 = nearNormal;
        Vec2f n2 = farNormal;
        if(t1 > t2)
        {
            std::swap(t1, t2);
            std::swap(n1, n2);
        }

        if(t1 > tEnter)
        {
            tEnter = t1;
            enterNormal = n1;
            hasEnterNormal = true;
        }
        tExit = std::min(tExit, t2);
        return tEnter <= tExit;
    };

    if(!clipAxis(o.x, delta.x, left, right, Vec2f{-1.f, 0.f}, Vec2f{1.f, 0.f}))
    {
        return std::nullopt;
    }
    if(!clipAxis(o.y, delta.y, top, bottom, Vec2f{0.f, -1.f}, Vec2f{0.f, 1.f}))
    {
        return std::nullopt;
    }

    if(!hasEnterNormal)
    {
        return std::nullopt;
    }
    if(tEnter < 0.f || tEnter > 1.f)
    {
        return std::nullopt;
    }

    return Hit{.time = tEnter, .normal = enterNormal};
}

Vec2f reflect(Vec2f velocity, Vec2f normal) noexcept
{
    const float d = dot(velocity, normal);
    if(d >= 0.f)
    {
        return velocity;
    }
    return velocity - 2.f * d * normal;
}

} // namespace sgl
