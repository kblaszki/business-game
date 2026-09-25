---
title: Engine collision
diataxis: reference
audience: [ai, human]
related_code:
  - engine/collision/include/sgl/collision/Circle.hpp
  - engine/collision/include/sgl/collision/Contact.hpp
  - engine/collision/include/sgl/collision/Hit.hpp
  - engine/collision/include/sgl/collision/Collision.hpp
  - engine/collision/src/Collision.cpp
  - engine/collision/CMakeLists.txt
  - tests/engine/collision/CollisionTest.cpp
related_docs:
  - engine-core.md
  - source-layout.md
keywords: [eng, collision, Circle, Aabb, Contact, Hit, intersect, sweep, reflect, Minkowski]
last_reviewed: 2026-09-25
---

# Engine collision

Static library target `sgl_collision` (`namespace sgl`), linked `PUBLIC` to `sgl_core`. No SFML. Include as `<sgl/collision/X.hpp>`.

Coordinate convention matches the legacy game and SFML: **+y points down**.

## Types

| Type | Role |
|------|------|
| `Circle` | `center` (`Vec2f`) + `radius` |
| `Aabb` | alias of `Rect<float>` (`pos` + `size`) |
| `Contact` | static overlap: outward `normal`, penetration `depth` |
| `Hit` | sweep result: entry `time` in `[0, 1]`, outward `normal` |

`sgl::detail::nearlyZero(v, eps = 1e-5f)` is used instead of `==` on floats.

## `intersect(circle, box)`

Nearest-point test of the circle center against the AABB.

- **Center outside:** nearest point on the box; `normal` is from nearest toward center (normalized); `depth` is `radius - distance`. Hit only when `distance <= radius`. Face overlaps give axis normals (`(0,+1)` from below, `(0,-1)` from above, `(-1,0)` / `(+1,0)` from the sides). Corner overlaps give a unit diagonal.
- **Center inside:** `normal` is the minimum-penetration axis (smallest distance to an edge), pointing outward; `depth` is that edge distance plus `radius`.
- Miss → `std::nullopt`.

## `sweep(circle, delta, box)`

Exact Minkowski sum of the AABB and a disk of `radius` (rounded rectangle):

1. Already overlapping → time `0` with the `intersect` normal.
2. Zero `delta` and no overlap → `std::nullopt`.
3. Slab-intersect the ray at `circle.center` with direction `delta` against the AABB **expanded by `radius`**.
4. If the entry sample lies in a **corner region** (outside the original box on both axes), replace the slab hit with a ray–circle test against the circle of radius `r` centered at that corner. Take the earliest valid root in `[0, 1]`; `normal` is `normalized(contactCenter - corner)`.
5. Otherwise (face region) keep the slab entry time and its outward **axis** normal.

Miss → `std::nullopt` (including paths that only clip an expanded corner square outside the quarter-circle).

## `reflect(velocity, normal)`

If `dot(velocity, normal) >= 0`, returns `velocity` unchanged (moving away or sliding). Otherwise returns `v - 2 * dot(v, n) * n`.
