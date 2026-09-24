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
  - src/world/PowerUp.hpp
  - src/world/PowerUp.cpp
  - src/world/LevelId.hpp
  - src/world/LevelDescriptor.hpp
  - src/world/LevelDescriptor.cpp
  - src/screen/GameplayScreen.hpp
  - src/screen/GameplayScreen.cpp
  - tests/unit_tests/WorldTest.cpp
  - tests/unit_tests/LevelDescriptorTest.cpp
  - tests/unit_tests/MenuGameplayTest.cpp
related_docs:
  - pause-overlay.md
  - power-ups.md
  - source-layout.md
  - input-and-events.md
  - ../../mvp/06-world-and-objects.md
  - ../../mvp/07-levels.md
  - ../../mvp/10-engine-progress.md
keywords: [World, Paddle, Ball, Brick, PowerUp, LevelId, Stage1, Stage2, Stage3, Breakout, score, lives]
last_reviewed: 2026-09-24
---

# World, objects, and levels

Facts about the running tree. Prospective extras (ECS, laser, a fifth level) stay in [`mvp/06`](../../mvp/06-world-and-objects.md) and [`mvp/07`](../../mvp/07-levels.md). Power-up capsule rules are also in [power-ups.md](power-ups.md).

## Ownership

`GameplayScreen` owns one `World` (by value). `World` owns `BreakoutArt` on the heap (`unique_ptr`) so move-assign (stage advance / restart) keeps paddle, ball, and brick sprites bound to live textures. It also owns a `Paddle`, `std::vector<Ball>`, `std::vector<Brick>`, and `std::vector<PowerUp>`. `ball()` is the first ball so older tests still compile; `ballCount()` / `ballAt` cover the rest. Menu and pause do not own a world. `World` does not see `Action` or the window.

`GameObject` is gone. Types are concrete: no virtual base, no ECS.

## Time

`GameplayScreen::update` forwards `FixedTimestep::tick` to `World::fixedUpdate`. Pause works because the overlay blocks that update. `World` has no pause flag.

When `won()` or `lost()`, later ticks still increment `tickCount` but skip paddle/ball motion. Already-spawned capsules still fall so a miss can expire.

Displacement stays `(vel / 60.f) * (tick / FixedTimestep::tick)` on paddle, balls, and capsules.

## Breakout rules

- Paletka: `setPaddleSpeed` from gameplay key hold (Left/Right/A/D). Clamp to design width. Default display width 120; Wide sets 200 via `Paddle::setDisplayWidth` (sprite scale) and restores 120 when the timer ends.
- Piłki: `World` starts with one `Stuck` ball on the paddle. `launch()` unsticks the first stuck ball and gives it `{160, -360}`. Lot, odbicia od ścian i paletki, jedna cegła na żywą piłkę na tick. Odbicie od cegły idzie na zewnątrz ściany kontaktu (normalna od najbliższego punktu AABB + `copysign`).
- Spód ekranu: życie spada tylko gdy **wszystkie** piłki mają `position().y >= 720`. Potem jedna `Stuck` na paletce. Jeśli spadnie tylko część, te piłki są usuwane i życie zostaje.
- 0 żywych cegieł (przy niepustej siatce) = `won()`. Pusty `World` (brak cegieł) ma `won() == false`.
- 0 żyć = `lost()`. Domyślnie 3 życia, wynik 0.
- Tekstury: `BreakoutArt` buduje `sf::Image` w kodzie. Bez PNG.

## Power-up donors

Each `addBrick` gets a sequential index starting at 0 (the brick’s index in the vector). A brick is a donor when `index % 4 == 0`. Kind is `static_cast<PowerUpKind>((index / 4) % 4)` in enum order Wide, MultiBall, Slow, ExtraLife.

Killing a donor (`bounceBricks` or `killBrick`) `emplace`s a `PowerUp` at the brick position. Each tick: `fixedUpdate` every alive capsule, erase dead ones (miss at `y >= 720`), AABB-overlap with the paddle calls `applyPowerUp` then erases the capsule.

Exactly one timed effect at a time (Wide and Slow share one 8 s / 480-tick timer). Picking Wide or Slow expires the previous timed effect first. MultiBall and ExtraLife do not clear that timer. `activePowerUpName()` is `"Wide"` or `"Slow"` while the timer runs, otherwise `""`.

## Levels

`LevelId` is `Stage1`, `Stage2`, or `Stage3`. `levelDescriptor` returns a 10×6 row-colored grid plus a row-major `mask` (`0` hole, `1` brick). Unknown ids fall back to `Stage1`.

- `Stage1`: full mask (60 bricks), same row colors as before.
- `Stage2`: checkerboard (`1` when `(row + col) % 2 == 0`).
- `Stage3`: border frame, hollow center.

`makeWorld` spawns a brick only where the mask is `1`. Menu still passes `Stage1`; it does not know coordinates.

## Stage advance

After `fixedUpdate`, if `world.won()` and the screen’s level is Stage1 or Stage2, `GameplayScreen` builds the next `LevelId` (Stage1→Stage2, Stage2→Stage3), `makeWorld(levelDescriptor(next))`, then `setScore` / `setLives` from the previous world. It does **not** show You win. Paddle-hold flags stay as they were.

If `world.won()` and the level is Stage3, the You win banner stays. Confirm retries the same Stage3 (fresh world, lives 3, score 0). Cancel → menu.

Lose is unchanged: You lose; Confirm retries the same level from scratch.
