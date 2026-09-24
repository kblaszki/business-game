---
title: Engine implementation progress
status: living
last_reviewed: 2026-09-24
related_docs:
  - README.md
  - 09-rollout.md
  - ../docs/explanation/architecture.md
  - ../docs/reference/source-layout.md
related_code:
  - ../engine/core/include/eng/core/Features.hpp
  - ../engine/input/include/eng/input/InputEvent.hpp
  - ../engine/scene/include/eng/scene/SceneStack.hpp
  - ../engine/loop/include/eng/loop/App.hpp
  - ../engine/render/include/eng/render/RenderQueue.hpp
  - ../engine/collision/include/eng/collision/Collision.hpp
  - ../engine/resources/include/eng/resources/ResourceCache.hpp
  - ../engine/sfml/include/eng/sfml/SfmlPlatform.hpp
  - ../games/arkanoid/sim/include/arkanoid/sim/State.hpp
  - ../games/arkanoid/app/include/arkanoid/app/Scenes.hpp
  - ../games/arkanoid/main.cpp
  - ../tests/engine/core/CMakeLists.txt
  - ../tests/arkanoid/sim/CMakeLists.txt
  - ../tests/arkanoid/app/CMakeLists.txt
---

# Engine implementation progress

This file is the **living register** of names and files that exist in the tree. [`01`](01-architecture.md)–[`09`](09-rollout.md) stay **prospective** (target design, wording “will”). Do not rewrite those chapters into “already implemented.” Facts about running code also belong in [`docs/`](../docs/index.md).

## Conventions in the tree

| Rule | In this tree |
|------|----------------|
| Interfaces | Suffix `FooI` (`SceneI`, `RendererI`, `PlatformI`, `ClockI`) |
| `mvp/` names with prefix `I` | Remap when that type lands — do not edit 01–09 |
| Directories | Engine split **landed**: `engine/` (`eng`) and `games/arkanoid/` (`arkanoid`, executable `arkanoid`). Contract: [`docs/explanation/architecture.md`](../docs/explanation/architecture.md) |
| SFML | Only `engine/sfml` and `games/arkanoid/main.cpp` include SFML. `arkanoid/sim` stays headless |
| CMake | Each module owns its `CMakeLists.txt` |

### Name remap (when those types are added)

| Locked in 01–09 | Name to use in the tree |
|-----------------|------------------------|
| `IScreen` | `SceneI` (landed) |
| (other `IFoo` from mvp) | `FooI` |

Until a row is implemented, the mvp spelling in 01–09 is still the design vocabulary for those chapters.

## Slice log

Check a box only after that slice is on `main`. Paths are what landed, not a promise of future files.

| # | Slice | Status | In the tree |
|---|-------|--------|-------------|
| 0–10 | Pre-cutover breakout under former `src/` | **superseded** | removed at cutover; historical notes below |
| 11 | Engine split | **done** | see Slice 11 |

Three stages and four power-ups live in `arkanoid_sim` / `arkanoid_app`. `01`–`09` stay prospective design notes.

### Slices 0–10 — pre-cutover (historical)

Slices 0–10 built the former `src/` breakout (`ScreenStack`, `World`, stages, power-ups). That tree is **gone**. Do not treat those paths as current. Detail lives in git history if needed; current facts are in [`docs/`](../docs/index.md) and Slice 11.

### Slice 11 — engine split (landed)

- [x] `eng_core` — Vec2, Rect, Handle, Image, Features
- [x] `eng_input` — InputEvent, ActionMap, InputState
- [x] `eng_scene` — SceneI, SceneStack, SceneRequest, SceneTraits
- [x] `eng_loop` — FixedStepLoop, App, PlatformI, ClockI
- [x] `eng_render` — RenderQueue, DrawCommand, Projection
- [x] `eng_collision` — intersect, sweep, reflect
- [x] `eng_resources` — ResourceCache, ResourceError
- [x] `eng_sfml` — SfmlPlatform, EventTranslate, SfmlRenderer, SfmlAssets, SfmlClock
- [x] `arkanoid_sim` — headless State / step / levels / power-ups
- [x] `arkanoid_app` — MainMenuScene, GameplayScene, PauseScene, HUD, assets, bindings
- [x] executable `arkanoid` — `games/arkanoid/main.cpp`
- [x] Headless tests under `tests/engine/` and `tests/arkanoid/` (`event_translate_test` links SFML::Window, opens no window)

```mermaid
flowchart LR
  Main[main.cpp] --> App[eng::App]
  Main --> Plat[SfmlPlatform]
  Main --> Stack[SceneStack]
  App --> Plat
  App --> Stack
  Stack --> Menu[MainMenuScene]
  Stack --> Play[GameplayScene]
  Stack --> Pause[PauseScene]
  Play --> Sim[arkanoid_sim]
```

| Path | Role |
|------|------|
| [`docs/explanation/architecture.md`](../docs/explanation/architecture.md) | Contract |
| [`docs/reference/source-layout.md`](../docs/reference/source-layout.md) | Targets and suites |
| [`docs/reference/arkanoid-sim.md`](../docs/reference/arkanoid-sim.md) | Sim rules |
| [`docs/reference/arkanoid-app.md`](../docs/reference/arkanoid-app.md) | Scenes and HUD |
