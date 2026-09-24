---
title: Power-ups
diataxis: reference
audience: [ai, human]
related_code:
  - src/world/PowerUp.hpp
  - src/world/PowerUp.cpp
  - src/world/World.hpp
  - src/world/World.cpp
  - src/screen/GameplayScreen.hpp
  - src/screen/GameplayScreen.cpp
  - tests/unit_tests/PowerUpTest.cpp
  - tests/unit_tests/WorldTest.cpp
related_docs:
  - world-and-levels.md
  - source-layout.md
keywords: [PowerUp, PowerUpKind, Wide, MultiBall, Slow, ExtraLife, capsule]
last_reviewed: 2026-09-24
---

# Power-ups

`World` owns `std::vector<PowerUp>`, spawns capsules from donor bricks, collects them on paddle overlap, and applies each kind. The capsule type itself is still a standalone falling sprite.

## Type

`PowerUp` is a concrete type (no virtual base). The constructor takes a spawn position and a `PowerUpKind`. Texture is a small generated `sf::Image` capsule in `PowerUp.cpp`, tinted by kind, stored as a process-static `sf::Texture` per kind (sprites must not own a texture that moves with `std::vector` reallocation or `erase`). No font or asset file.

| Kind | Tint role |
|------|-----------|
| `Wide` | Gold capsule |
| `MultiBall` | Cyan capsule |
| `Slow` | Blue capsule |
| `ExtraLife` | Green capsule |

`kind()` returns the value passed to the constructor. `size()` is the capsule sprite’s local bounds.

## Motion

Falls downward at **180 px/s**. Displacement matches paddle/ball: `(vel / 60.f) * (tick / FixedTimestep::tick)`.

Dies when `position().y >= 720` (`Game::DESIGN_SIZE.y`). `alive()` is then false. Later `fixedUpdate` calls are no-ops. `draw` skips a dead capsule. `World` then erases it (`powerUpCount() == 0` after a miss).

## Spawn and catch

Each `addBrick` index starting at 0: donor when `index % 4 == 0`. Kind = `static_cast<PowerUpKind>((index / 4) % 4)` in enum order Wide, MultiBall, Slow, ExtraLife. Killing a donor (collision or `killBrick`) spawns a capsule at the brick position.

Each tick: `fixedUpdate` every alive capsule; erase dead ones; AABB-overlap with the paddle collects it (`applyPowerUp(kind)` then erase). Capsules still fall after `won()` / `lost()` so a missed donor can reach `y >= 720` and be dropped.

## Effects (`World::applyPowerUp`)

Exactly one timed effect at a time. Wide and Slow share one 8 s timer (480 ticks). Picking Wide or Slow expires the previous timed effect first, then starts the new one. MultiBall and ExtraLife do not replace or clear that timer. `activePowerUpName()` returns `"Wide"` or `"Slow"` while it runs, otherwise `""`. Gameplay HUD shows that string top-center.

| Kind | Effect |
|------|--------|
| `Wide` | Paddle display width 200 (default 120) for 8 s. When it ends, restore 120 and clamp X. |
| `Slow` | Multiply every current ball velocity by 0.6 for 8 s. A `slowActive` flag prevents stacking 0.6 twice. New MultiBall extras spawned while Slow is on are already scaled. When Slow ends, divide live-ball velocities by 0.6. |
| `ExtraLife` | `lives + 1`, no timer. |
| `MultiBall` | Two extra balls from the first ball (same position). If that ball is Live, copy its velocity and set extras to `vel.x - 120` and `vel.x + 120` (same y). If Stuck, extras are not stuck and use `{160-120,-360}` and `{160+120,-360}`. |

## Tests

`power_up_test` spawns at `{100, 100}` and checks `y` increases across ticks, places a capsule near the bottom so one tick sets `alive()` false, and reallocates a `vector<PowerUp>` so the first capsule still reports kind and pose.

`world_test` covers `applyPowerUp(Wide)` widening the paddle, `ExtraLife` going to 4 lives, and a missed donor capsule reaching `powerUpCount() == 0`.
