---
title: Engine render queue and projections
diataxis: reference
audience: [ai, human]
related_code:
  - engine/render/include/eng/render/AssetIds.hpp
  - engine/render/include/eng/render/DrawCommand.hpp
  - engine/render/include/eng/render/RenderQueue.hpp
  - engine/render/src/RenderQueue.cpp
  - engine/render/include/eng/render/RendererI.hpp
  - engine/render/include/eng/render/Projection.hpp
  - engine/render/include/eng/render/Camera.hpp
  - tests/mocks/RendererMock.hpp
  - tests/engine/render/RenderQueueTest.cpp
  - tests/engine/render/ProjectionTest.cpp
related_docs:
  - source-layout.md
  - ../explanation/architecture.md
keywords: [eng, RenderQueue, RendererI, DrawCommand, SortKey, Layer, Projection, Orthographic, Isometric, Camera]
last_reviewed: 2026-09-24
---

# Engine render queue and projections

CPU-side draw list and 2.5D helpers in `namespace eng`, included as `<eng/render/X.hpp>`. Target `eng_render` is a STATIC library that PUBLIC-links `eng_core`. No SFML dependency.

## Asset ids

`TextureId` / `FontId` are `Handle<TextureTag>` / `Handle<FontTag>` from `AssetIds.hpp`. Resource backends own the tag types; they do not redefine the aliases.

## Draw commands

`DrawCommand` is `std::variant<SpriteCmd, RectCmd, TextCmd>`.

| Type | Role |
|------|------|
| `SpriteCmd` | textured quad: `texture`, `source`, `position`, `scale`, `tint` |
| `RectCmd` | filled/outlined AABB: `rect`, `fill`, `outline`, `outlineThickness` |
| `TextCmd` | string: `font`, `text`, `size`, `position`, `color`, `anchor` (`TopLeft` / `Center`) |

`Entry` pairs a `SortKey` with a `DrawCommand`. `SortKey` orders by `Layer` (Background → Overlay), then `depth`, then `seq`.

## RenderQueue

- `push(layer, depth, command)` — assigns `seq` incrementally starting at 0
- `sorted()` — `std::ranges::stable_sort` by `SortKey` once; stays cached until the next `push` or `clear`
- `size()` / `clear()` — `clear` empties entries and resets `seq` to 0

## RendererI

Port for a backend: `begin()`, `submit(const RenderQueue&)`, `end()`. Tests use `RendererMock` (`tests/mocks/RendererMock.hpp`).

## Projection and camera

`Projection` requires `project(Vec3f) -> Projected` (`screen`, `depth`).

| Type | Mapping |
|------|---------|
| `Orthographic` | screen `(x, y)`, depth `z` |
| `Isometric` | screen `((x-y)*tileW/2, (x+y)*tileH/2 - z*tileH)`, depth `x+y+z*0.001` |

`Camera::apply(screen)` returns `(screen - offset) * zoom`.
