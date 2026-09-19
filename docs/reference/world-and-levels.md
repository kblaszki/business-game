---
title: World and levels
diataxis: reference
audience: [ai, human]
related_code:
  - src/World.hpp
  - src/World.cpp
  - src/GameObject.hpp
  - src/GameObject.cpp
  - src/LevelId.hpp
  - src/LevelDescriptor.hpp
  - src/makeWorld.hpp
  - src/makeWorld.cpp
  - src/GameplayScreen.cpp
  - src/Game.hpp
related_docs:
  - application-loop.md
  - screens-and-input.md
  - source-layout.md
keywords: [World, GameObject, LevelId, LevelDescriptor, makeWorld, sandbox, wrap]
last_reviewed: 2026-09-19
---

# World and levels

`GameplayScreen` owns the `World`. It builds one with `makeWorld(levelDescriptor(id))`. `Game` does not load levels or call `World::fixedUpdate`.

`World` has no pause flag. Ticks stop because `Game::run` skips `drain` / `stack.update` while `pauseIsTop()` (see [application-loop.md](application-loop.md)).

## Types

| Type | Role |
|------|------|
| `LevelId` | `Sandbox` only |
| `SpawnSpec` | `position`, `velocity` |
| `LevelDescriptor` | `id` plus a `span` of `SpawnSpec` |
| `levelDescriptor(id)` | Returns the static sandbox table; other ids are unreachable |
| `makeWorld` | Immediate `World::spawn` of one `GameObject` per spec |
| `World` | Owns `unique_ptr<GameObject>`s; `fixedUpdate` / const `draw`; `dummyPosition` for tests |
| `GameObject` | Yellow 40×40 `sf::RectangleShape`; wrap motion in `DESIGN_SIZE` |

There is no deferred spawn/despawn, `restart()`, player steering, or `Action::MoveDummy`.

## Sandbox dummy

| Field | Value |
|-------|-------|
| Size | 40×40 |
| Start | `{0, 340}` |
| Velocity | `{240, 0}` px/s |
| Motion | wrap each axis in `Game::DESIGN_SIZE` (1280×720), not bounce |

Each `fixedUpdate` steps `velocity * (1/60) * (tick / 1s/60)` so the step stays exact when `tick` is the 1/60 s constant.
