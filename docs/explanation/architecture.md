---
title: Engine architecture decisions
diataxis: explanation
audience: [ai, human]
related_code:
  - CMakeLists.txt
related_docs:
  - roadmap.md
  - ../reference/source-layout.md
  - ../../mvp/10-engine-progress.md
keywords: [architecture, ADR, namespace, eng, arkanoid, ECS, 2.5D, boundaries]
last_reviewed: 2026-09-24
---

# Engine architecture decisions

Locked contract for the engine split in **sfml-game-lab**. The running tree is `engine/` and `games/arkanoid/`; further games are siblings under `games/<title>/`. This page is the source of truth for boundaries and names.

## Layout and names

| Decision | Choice |
|----------|--------|
| Engine root | `engine/` |
| First game | `games/arkanoid/` |
| Engine namespace | `eng` |
| SFML backend | `eng::sfml` |
| Engine internals | `eng::detail` |
| Game namespace | `arkanoid` |
| Includes | `<eng/<module>/X.hpp>`, `<arkanoid/<sim\|app>/X.hpp>` |
| Interfaces | Suffix `FooI` (`SceneI`, `RendererI`, `PlatformI`, `ClockI`) |

Modules are directories, not nested namespaces. Do not write `eng::render::RenderQueue`.

## Targets

`eng_core` (headers), `eng_input`, `eng_scene`, `eng_loop`, `eng_render`, `eng_collision`, `eng_resources`, `eng_sfml` (the only engine target that links SFML), `arkanoid_sim`, `arkanoid_app`, executable `arkanoid`.

Link direction: input and collision and resources and render depend on core; scene depends on input; loop depends on scene and render; sfml depends on loop and resources; `arkanoid_sim` depends on collision; `arkanoid_app` depends on sim, loop, and resources; the executable links `arkanoid_app` and `eng_sfml`.

## Boundaries

- Only `engine/sfml` and `games/arkanoid/main.cpp` may include `<SFML/...>`.
- `engine/**` never includes `arkanoid/**`.
- `arkanoid/sim` includes only `eng/core` and `eng/collision`. Input is `SimInput`; output is `std::vector<SimEvent>`.
- Each module owns its `CMakeLists.txt`. Parallel agents do not edit a shared sources list.

## Simulation vs presentation

Simulation stores positions and sizes as values (`eng::Vec2f`, `eng::Rect`). Sprites do not own simulation state. Rendering reads a snapshot and pushes `DrawCommand` into `RenderQueue`, sorted by `SortKey` (layer, depth, sequence).

## 2.5D

`Projection` is a concept. `Orthographic` and `Isometric` map `Vec3` to screen position plus depth. Arkanoid uses `Orthographic` and layers Background, World, Actors, Hud, Overlay.

## ECS

No entity-component registry in this refactor. Entities are plain structs in vectors, updated by free functions. Dynamic polymorphism stays on ports (`SceneI`, `RendererI`, `PlatformI`, `ClockI`). Revisit a registry only when a game grows past a handful of component combinations.

## C++23 subset

Allowed: `std::expected`, `std::optional`, `std::variant`, `std::span`, ranges and views, concepts, `std::move_only_function`, `std::to_underlying`, `<=>`, `constexpr`.

Forbidden: `std::mdspan`, `std::print`, exceptions for control flow, raw `new`/`delete`, function-local static GPU resources.

CI uses GCC 14 and MSVC so `std::expected` and `std::move_only_function` are available.
