---
title: World, objects, and levels
diataxis: reference
audience: [ai, human]
related_code:
  - src/world/World.hpp
  - src/world/World.cpp
  - src/world/Paddle.hpp
  - src/world/Paddle.cpp
  - src/world/Ball.hpp
  - src/world/Ball.cpp
  - src/world/Brick.hpp
  - src/world/Brick.cpp
  - src/world/BreakoutArt.hpp
  - src/world/BreakoutArt.cpp
  - src/world/LevelId.hpp
  - src/world/LevelDescriptor.hpp
  - src/world/LevelDescriptor.cpp
  - src/screen/GameplayScreen.hpp
  - src/screen/GameplayScreen.cpp
  - tests/unit_tests/WorldTest.cpp
  - tests/unit_tests/LevelDescriptorTest.cpp
related_docs:
  - pause-overlay.md
  - source-layout.md
  - ../../mvp/06-world-and-objects.md
  - ../../mvp/07-levels.md
  - ../../mvp/10-engine-progress.md
keywords: [World, Paddle, Ball, Brick, LevelId, Stage1, Breakout, score, lives]
last_reviewed: 2026-09-24
---

# World, objects, and levels

Facts about the running tree. Prospective extras (power-ups, a second stage, ECS) stay in [`mvp/06`](../../mvp/06-world-and-objects.md) and [`mvp/07`](../../mvp/07-levels.md).

## Ownership

`GameplayScreen` owns one `World` (by value). `World` owns a `Paddle`, a `Ball`, and a vector of `Brick`. Menu and pause do not own a world. `World` does not see `Action` or the window.

`GameObject` is gone. Types are concrete: no virtual base, no ECS.

## Time

`GameplayScreen::update` forwards `FixedTimestep::tick` to `World::fixedUpdate`. Pause works because the overlay blocks that update. `World` has no pause flag.

When `won()` or `lost()`, later ticks still increment `tickCount` but skip motion.

## Breakout rules

- Paletka: `setPaddleSpeed` from gameplay key hold (Left/Right/A/D). Clamp to design width.
- Piłka: `Stuck` na paletce aż `launch()` (Enter). Lot, odbicia od ścian i paletki, jedna cegła na tick. Odbicie od cegły idzie na zewnątrz ściany kontaktu (normalna od najbliższego punktu AABB).
- Spód ekranu: −1 życie, znowu `Stuck`. 0 żyć = `lost()`.
- 0 żywych cegieł (przy niepustej siatce) = `won()`.
- Tekstury: `BreakoutArt` buduje `sf::Image` w kodzie. Bez PNG.

## Levels

`LevelId::Stage1` + `levelDescriptor` (10×6, kolory wierszy). `makeWorld` stawia paletkę, przyklejoną piłkę i cegły. Menu podaje `Stage1`; nie zna współrzędnych.
