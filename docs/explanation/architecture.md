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
keywords: [architecture, ADR, namespace, sgl, arkanoid, tetris, ECS, 2.5D, boundaries, audio]
last_reviewed: 2026-09-25
---

# Engine architecture decisions

Locked contract for **sfml-game-lab**. The running tree is `engine/` plus games under `games/<title>/`. This page is the source of truth for boundaries and names.

## Layout and names

| Decision | Choice |
|----------|--------|
| Engine root | `engine/` |
| Engine namespace | `sgl` (no nested `sgl::eng`; the directory already separates the engine) |
| SFML backend | `sgl::sfml` |
| Engine internals | `sgl::detail` |
| Games | `sgl::arkanoid`, `sgl::tetris` |
| Includes | `<sgl/<module>/X.hpp>`, `<arkanoid/<sim\|app>/X.hpp>`, `<tetris/<sim\|app>/X.hpp>` |
| Interfaces | Suffix `FooI` (`SceneI`, `RendererI`, `PlatformI`, `ClockI`, `AudioI`) |

Modules are directories, not nested namespaces. Do not write `sgl::render::RenderQueue`.

## Targets

`sgl_core` (headers), `sgl_input`, `sgl_scene`, `sgl_loop`, `sgl_render`, `sgl_collision`, `sgl_resources`, `sgl_audio`, `sgl_fx`, `sgl_save`, `sgl_sfml` (the only engine target that links SFML), `arkanoid_sim`, `arkanoid_app`, executable `arkanoid`, `tetris_sim`, `tetris_app`, executable `tetris`.

Link direction: input, collision, resources, and render depend on core; scene depends on input; loop depends on scene and render; audio and save depend on core; fx depends on core and render; sfml depends on loop, resources, and audio; `arkanoid_sim` depends on collision; `tetris_sim` depends on core only; each app depends on its sim, loop, resources, audio, fx, and save; each executable links its app and `sgl_sfml`.

## Boundaries

- Only `engine/sfml` and `games/<game>/main.cpp` may include `<SFML/...>`.
- `engine/**` never includes a game header.
- `arkanoid/sim` includes only `sgl/core` and `sgl/collision`. Input is `SimInput`; output is `std::vector<SimEvent>`.
- `tetris/sim` includes only `sgl/core`.
- Each module owns its `CMakeLists.txt`. Parallel agents do not edit a shared sources list.

## Simulation vs presentation

Simulation stores positions and sizes as values (`sgl::Vec2f`, `sgl::Rect`). Sprites do not own simulation state. Rendering reads a snapshot and pushes `DrawCommand` into `RenderQueue`, sorted by `SortKey` (layer, depth, sequence).

## 2.5D

`Projection` is a concept. `Orthographic` and `Isometric` map `Vec3` to screen position plus depth. Arkanoid uses `Orthographic` and layers Background, World, Actors, Hud, Overlay.

## ECS

No entity-component registry in this refactor. Entities are plain structs in vectors, updated by free functions. Dynamic polymorphism stays on ports (`SceneI`, `RendererI`, `PlatformI`, `ClockI`). Revisit a registry only when a game grows past a handful of component combinations.

## C++23 subset

Allowed: `std::expected`, `std::optional`, `std::variant`, `std::span`, ranges and views, concepts, `std::move_only_function`, `std::to_underlying`, `<=>`, `constexpr`.

Forbidden: `std::mdspan`, `std::print`, exceptions for control flow, raw `new`/`delete`, function-local static GPU or audio resources.

CI uses GCC 14 and MSVC so `std::expected` and `std::move_only_function` are available.

## Phase II

### Input edges

`pressed`, `released`, and `focusLost` survive a frame that runs zero fixed steps. `App` clears them at the start of the next frame only after a frame that ran at least one scene update. Within a multi-step frame they are visible on the first step only. `held` for an action stays true while any bound key is down.

### Randomness

Simulation and particle code use `sgl::Pcg32` only. `std::uniform_int_distribution`, `std::shuffle`, `rand`, and wall-clock time are forbidden there, because libstdc++ and MSVC disagree on the standard distributions.

### Effects

Timed effects are data (`std::variant` values with a remaining duration). Derived quantities (paddle width, ball speed) are pure functions of that data. Code does not mutate a value and later undo it.

### Audio, particles, scores

`AudioI::play` is the port. `sgl_audio` stays free of SFML and builds PCM with `tone`. `sgl_sfml` implements the port and links `SFML::Audio`. `sgl_fx` is a fixed-capacity particle system. `sgl_save` stores a descending high-score table and writes it with a temp file plus rename.

### Tetris

`games/tetris/sim` owns the 10×40 grid (20 visible rows), SRS kicks, the 7-bag, gravity, lock delay, hold, and DAS/ARR in simulation time. `games/tetris/app` maps `InputState` to `TetrisInput`, draws with `DrawCommand`, plays events through `AudioI`, and writes `tetris.scores`. The executable is `tetris`.
