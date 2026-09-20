---
title: World and levels
diataxis: reference
audience: [ai, human]
related_code:
  - src/World.hpp
  - src/World.cpp
  - src/EntityI.hpp
  - src/CollidableI.hpp
  - src/Paddle.hpp
  - src/Paddle.cpp
  - src/Ball.hpp
  - src/Ball.cpp
  - src/Brick.hpp
  - src/Brick.cpp
  - src/RectCollision.hpp
  - src/LevelId.hpp
  - src/LevelDescriptor.hpp
  - src/makeWorld.hpp
  - src/makeWorld.cpp
  - src/GameplayScreen.cpp
  - src/window/DrawerI.hpp
  - src/Game.hpp
related_docs:
  - application-loop.md
  - screens-and-input.md
  - source-layout.md
keywords: [World, EntityI, CollidableI, Paddle, Ball, Brick, LevelId, Arkanoid]
last_reviewed: 2026-09-20
---

# World and levels

`GameplayScreen` owns the `World`. It builds one with `makeWorld(levelDescriptor(id))`. `Game` does not load levels or call `World::fixedUpdate`. `Paddle` / `Ball` / `Brick` keep pose in `sf::Vector2f` (and brick fill color). `draw` builds a local `RectangleShape` / `CircleShape` and calls `DrawerI::draw`. Production draws through `WindowI`; unit tests use `NullDrawer` / `DrawerMock` so they never call `sf::RenderTarget::draw`.

`World` has no pause flag. Ticks stop because `Game::run` skips `drain` / `stack.update` while `pauseIsTop()` (see [application-loop.md](application-loop.md)). Collision and lives live on `GameplayScreen`, not on `World`.

## Types

| Type | Role |
|------|------|
| `LevelId` | `Arkanoid` only |
| `LevelDescriptor` | `{ id }` — grid constants live in `makeWorld` |
| `makeWorld` | Spawns paddle, ball, then 50 bricks |
| `World` | Owns `unique_ptr<EntityI>`s; `objectAt`; draw skips `!alive()`; collisions via `dynamic_cast<CollidableI*>` |
| `EntityI` | `fixedUpdate`, `draw`, `alive()` (default true). No pose — menu `Button` can share this later. |
| `CollidableI` | `EntityI` plus `position` / `bounds`. `Paddle` / `Ball` / `Brick`. |
| `Paddle` | Green `100×20`, start `{590, 680}`, `600` px/s, clamp X `[0, 1180]`; pose is `Vector2f`, not `RectangleShape` |
| `Ball` | White circle, radius `8`, launch `{252, -420}`; walls L/R/T; floor sets `lost()`; pose is center `Vector2f` |
| `Brick` | `80×30`, one-hit (`destroy()` / `alive()`); pose is `Vector2f` + `Color` |
| `RectCollision` | AABB overlap; shallow-axis brick bounce; paddle ±60° |

There is no score, font HUD, brick HP, deferred spawn, or `Action::MoveDummy`. Paddle hold is `GameplayScreen::handleEvent` Left/Right.

## Session

- Lives start at `3`. Floor hit: `--lives`, then `resetAbove(paddle)` or `requestReplace(MainMenuScreen)` at 0.
- Win: `remainingBricks() == 0` → replace menu.
- Lives HUD: white 16×16 pips at `{16, 12}`, one per remaining life.

## Brick grid

`10×5`, gap `4`, top `60`, start X `222` (`(1280 - 836) / 2`). Row colors: `{220,80,80}`, `{220,160,60}`, `{220,220,80}`, `{80,180,80}`, `{80,140,220}`.
