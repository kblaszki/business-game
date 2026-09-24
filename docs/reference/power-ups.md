---
title: Power-ups
diataxis: reference
audience: [ai, human]
related_code:
  - src/world/PowerUp.hpp
  - src/world/PowerUp.cpp
  - tests/unit_tests/PowerUpTest.cpp
related_docs:
  - world-and-levels.md
  - source-layout.md
keywords: [PowerUp, PowerUpKind, Wide, MultiBall, Slow, ExtraLife, capsule]
last_reviewed: 2026-09-24
---

# Power-ups

Standalone falling capsule. `World` does not own, spawn, or pick it up yet. Gameplay effects of each kind are not applied.

## Type

`PowerUp` is a concrete type (no virtual base). The constructor takes a spawn position and a `PowerUpKind`. Texture is a small generated `sf::Image` capsule in `PowerUp.cpp`, tinted by kind. No font or asset file.

| Kind | Tint role |
|------|-----------|
| `Wide` | Gold capsule |
| `MultiBall` | Cyan capsule |
| `Slow` | Blue capsule |
| `ExtraLife` | Green capsule |

`kind()` returns the value passed to the constructor. `size()` is the capsule sprite’s local bounds.

## Motion

Falls downward at **180 px/s**. Displacement matches paddle/ball: `(vel / 60.f) * (tick / FixedTimestep::tick)`.

Dies when `position().y >= 720` (`Game::DESIGN_SIZE.y`). `alive()` is then false. Later `fixedUpdate` calls are no-ops. `draw` skips a dead capsule.

## Tests

`power_up_test` spawns at `{100, 100}` and checks `y` increases across ticks, and places a capsule near the bottom so one tick sets `alive()` false.
