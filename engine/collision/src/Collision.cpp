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

struct SlabHit
{
    float tEnter{};
    float tExit{};
    Vec2f enterNormal{};
};

std::optional<SlabHit> slabIntersectExpanded(Vec2f origin, Vec2f delta, const Aabb& box, float radius) noexcept
{
    const float left = box.pos.x - radius;
    const float right = box.pos.x + box.size.x + radius;
    const float top = box.pos.y - radius;
    const float bottom = box.pos.y + box.size.y + radius;

    float tEnter = -std::numeric_limits<float>::infinity();
    float tExit = std::numeric_limits<float>::infinity();
    Vec2f enterNormal{0.f, 0.f};
    bool hasEnterNormal = false;

    auto clipAxis =
        [&](float o, float dir, float nearPlane, float farPlane, Vec2f nearNormal, Vec2f farNormal) -> bool {
        if(detail::nearlyZero(dir))
        {
            return !(o < nearPlane || o > farPlane);
        }

        const float inv = 1.f / dir;
        float t1 = (nearPlane - o) * inv;
        float t2 = (farPlane - o) * inv;
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

    if(!clipAxis(origin.x, delta.x, left, right, Vec2f{-1.f, 0.f}, Vec2f{1.f, 0.f}))
    {
        return std::nullopt;
    }
    if(!clipAxis(origin.y, delta.y, top, bottom, Vec2f{0.f, -1.f}, Vec2f{0.f, 1.f}))
    {
        return std::nullopt;
    }
    if(!hasEnterNormal)
    {
        return std::nullopt;
    }
    return SlabHit{.tEnter = tEnter, .tExit = tExit, .enterNormal = enterNormal};
}

std::optional<Hit> rayCircleHit(Vec2f origin, Vec2f delta, Vec2f corner, float radius) noexcept
{
    const Vec2f f = origin - corner;
    const float a = dot(delta, delta);
    if(detail::nearlyZero(a))
    {
        return std::nullopt;
    }

    const float b = 2.f * dot(f, delta);
    const float c = dot(f, f) - radius * radius;
    const float disc = b * b - 4.f * a * c;
    if(disc < 0.f)
    {
        return std::nullopt;
    }

    const float sqrtDisc = std::sqrt(disc);
    const float inv2a = 0.5f / a;
    const float t0 = (-b - sqrtDisc) * inv2a;
    const float t1 = (-b + sqrtDisc) * inv2a;

    float t = std::numeric_limits<float>::infinity();
    if(t0 >= 0.f && t0 <= 1.f)
    {
        t = t0;
    }
    if(t1 >= 0.f && t1 <= 1.f && t1 < t)
    {
        t = t1;
    }
    if(!(t <= 1.f))
    {
        return std::nullopt;
    }

    const Vec2f contactCenter = origin + delta * t;
    const auto normal = normalized(contactCenter - corner);
    if(!normal)
    {
        return std::nullopt;
    }
    return Hit{.time = t, .normal = *normal};
}

std::optional<Vec2f> cornerForPoint(Vec2f p, const Aabb& box) noexcept
{
    const float left = box.pos.x;
    const float top = box.pos.y;
    const float right = box.pos.x + box.size.x;
    const float bottom = box.pos.y + box.size.y;

    const bool leftOf = p.x < left;
    const bool rightOf = p.x > right;
    const bool above = p.y < top;
    const bool below = p.y > bottom;

    if(leftOf && above)
    {
        return Vec2f{left, top};
    }
    if(rightOf && above)
    {
        return Vec2f{right, top};
    }
    if(leftOf && below)
    {
        return Vec2f{left, bottom};
    }
    if(rightOf && below)
    {
        return Vec2f{right, bottom};
    }
    return std::nullopt;
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

    if(detail::nearlyZero(delta.x) && detail::nearlyZero(delta.y))
    {
        return std::nullopt;
    }

    const float r = circle.radius;
    const Vec2f o = circle.center;
    const auto slab = slabIntersectExpanded(o, delta, box, r);
    if(!slab)
    {
        return std::nullopt;
    }
    if(slab->tExit < 0.f || slab->tEnter > 1.f)
    {
        return std::nullopt;
    }

    // Entry sample: slab entry when approaching from outside; otherwise origin (corner pocket).
    const Vec2f sample = (slab->tEnter >= 0.f) ? (o + delta * slab->tEnter) : o;
    if(const auto corner = cornerForPoint(sample, box))
    {
        return rayCircleHit(o, delta, *corner, r);
    }

    if(slab->tEnter < 0.f)
    {
        return std::nullopt;
    }
    return Hit{.time = slab->tEnter, .normal = slab->enterNormal};
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
