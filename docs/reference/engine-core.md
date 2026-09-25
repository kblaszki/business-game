---
title: Engine core value types
diataxis: reference
audience: [ai, human]
related_code:
  - engine/core/include/sgl/core/Vec2.hpp
  - engine/core/include/sgl/core/Vec3.hpp
  - engine/core/include/sgl/core/Rect.hpp
  - engine/core/include/sgl/core/Color.hpp
  - engine/core/include/sgl/core/Time.hpp
  - engine/core/include/sgl/core/Handle.hpp
  - engine/core/include/sgl/core/EntityId.hpp
  - engine/core/include/sgl/core/Image.hpp
  - tests/engine/core/VecTest.cpp
  - tests/engine/core/RectTest.cpp
  - tests/engine/core/HandleTest.cpp
  - tests/engine/core/ImageTest.cpp
related_docs:
  - source-layout.md
keywords: [eng, Vec2, Vec3, Vec2f, Vec2u, Vec3f, Rect, Color, Time, Seconds, kTick, Handle, EntityId, Image]
last_reviewed: 2026-09-25
---

# Engine core value types

Header-only POD-style types in `namespace sgl`, included as `<sgl/core/X.hpp>`. Target `sgl_core` is an INTERFACE library; no SFML dependency.

## Vectors

`Vec2<T>` / `Vec3<T>` require `std::floating_point T`. Aliases: `Vec2f`, `Vec3f`. `Vec2u` is a separate integer size type (`x`, `y` as `uint32_t`) used for design resolutions.

- Arithmetic: `+`, `-`, `*` (scalar either side), `+=`
- Comparison: defaulted `==` and `<=>`
- Free functions: `dot`, `lengthSq`, `normalized` (returns `std::nullopt` when `lengthSq` is zero)

## Rect

`Rect<T>` holds `pos` and `size` as `Vec2<T>`.

- `contains(point)` — half-open `[pos, pos + size)`
- `intersects(other)` — strict area overlap; edge-only contact is not an intersection

## Color and Time

- `Color` — `r`, `g`, `b`, `a` (`a` defaults to 255); constexpr-friendly; defaulted `==`
- `Seconds` — `std::chrono::duration<float>`; `kTick` is `1/60` seconds

## Handles and entities

- `Handle<Tag>` — opaque `uint32_t id` with defaulted `<=>`; `std::hash` specialized for use in unordered containers
- `EntityId` — `index` + `generation`; defaulted `==`

## Image

CPU pixel buffer: `width`, `height`, `std::vector<Color> pixels`.

- `Image::filled(w, h, c)` — allocate and fill
- `at(x, y)` — `std::nullopt` out of range
- `set(x, y, c)` — `false` out of range; otherwise writes and returns `true`

Row-major indexing: `y * width + x`.
