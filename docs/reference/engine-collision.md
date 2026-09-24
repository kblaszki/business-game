---
title: Engine collision
diataxis: reference
audience: [ai, human]
related_code:
  - engine/collision/include/eng/collision/Circle.hpp
  - engine/collision/include/eng/collision/Contact.hpp
  - engine/collision/include/eng/collision/Hit.hpp
  - engine/collision/include/eng/collision/Collision.hpp
  - engine/collision/src/Collision.cpp
  - engine/collision/CMakeLists.txt
  - tests/engine/collision/CollisionTest.cpp
related_docs:
  - engine-core.md
  - source-layout.md
keywords: [eng, collision, Circle, Aabb, Contact, Hit, intersect, sweep, reflect, Minkowski]
last_reviewed: 2026-09-24
---

# Engine collision

Static library target `eng_collision` (`namespace eng`), linked `PUBLIC` to `eng_core`. No SFML. Include as `<eng/collision/X.hpp>`.

Coordinate convention matches the legacy game and SFML: **+y points down**.

## Types

| Type | Role |
|------|------|
| `Circle` | `center` (`Vec2f`) + `radius` |
| `Aabb` | alias of `Rect<float>` (`pos` + `size`) |
| `Contact` | static overlap: outward `normal`, penetration `depth` |
| `Hit` | sweep result: entry `time` in `[0, 1]`, outward axis `normal` |

`eng::detail::nearlyZero(v, eps = 1e-5f)` is used instead of `==` on floats.

## `intersect(circle, box)`

Nearest-point test of the circle center against the AABB.

- **Center outside:** nearest point on the box; `normal` is from nearest toward center (normalized); `depth` is `radius - distance`. Hit only when `distance <= radius`. Face overlaps give axis normals (`(0,+1)` from below, `(0,-1)` from above, `(-1,0)` / `(+1,0)` from the sides). Corner overlaps give a unit diagonal.
- **Center inside:** `normal` is the minimum-penetration axis (smallest distance to an edge), pointing outward; `depth` is that edge distance plus `radius`.
- Miss → `std::nullopt`.

## `sweep(circle, delta, box)`

Ray at `circle.center` with direction `delta` against the AABB **expanded by `radius`** (Minkowski sum approximated as an expanded box). **Corners are treated as part of the expanded box**, not as rounded quarter-circles — a known approximation.

Returns the earliest entry time in `[0, 1]` with an outward **axis** normal. If already overlapping at `t = 0`, returns time `0` with the `intersect` normal. Miss → `std::nullopt`.

## `reflect(velocity, normal)`

If `dot(velocity, normal) >= 0`, returns `velocity` unchanged (moving away or sliding). Otherwise returns `v - 2 * dot(v, n) * n`.
