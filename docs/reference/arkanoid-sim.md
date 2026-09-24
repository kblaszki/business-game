---
title: Arkanoid sim
diataxis: reference
audience: [ai, human]
related_code:
  - games/arkanoid/sim/include/arkanoid/sim/Tuning.hpp
  - games/arkanoid/sim/include/arkanoid/sim/State.hpp
  - games/arkanoid/sim/include/arkanoid/sim/Events.hpp
  - games/arkanoid/sim/include/arkanoid/sim/Levels.hpp
  - games/arkanoid/sim/include/arkanoid/sim/Physics.hpp
  - games/arkanoid/sim/src/Levels.cpp
  - games/arkanoid/sim/src/Physics.cpp
  - games/arkanoid/sim/CMakeLists.txt
  - tests/arkanoid/sim/SimTest.cpp
  - tests/arkanoid/sim/CMakeLists.txt
related_docs:
  - engine-collision.md
  - engine-core.md
  - world-and-levels.md
  - power-ups.md
  - source-layout.md
  - ../explanation/architecture.md
keywords: [arkanoid, sim, State, step, SimInput, SimEvent, makeState, PowerUpKind, StageId, sweep]
last_reviewed: 2026-09-24
---

# Arkanoid sim

Headless breakout rules in `namespace arkanoid`. Static library target `arkanoid_sim` links `PUBLIC` `eng_collision` (which pulls `eng_core`). No SFML. Include as `<arkanoid/sim/X.hpp>`.

Design space is **1280×720** with **+y down**, matching the legacy `World` donor under `src/world/`.

## Types

| Header | Contents |
|--------|----------|
| `Tuning.hpp` | Inline constexpr design size, paddle/ball/brick/capsule sizes and speeds, effect duration 8 s, slow factor 0.6, multiball spread 120, score 10, lives 3, grid 10×6 |
| `State.hpp` | `Ball` / `Paddle` / `Brick` / `Capsule` / `Effects` / `State`; enums `BallMode`, `PowerUpKind`, `StageId` |
| `Events.hpp` | `BrickDestroyed`, `LifeLost`, `StageCleared`, `GameOver`, `PowerUpCaught`; `SimEvent` variant |
| `Levels.hpp` | `LevelData`, `level(StageId)`, `makeState` |
| `Physics.hpp` | `SimInput`, `step`, `applyPowerUp` |

`Ball::pos` is the top-left of a 16×16 box (radius 8). Paddle y is fixed at 680.

## Levels and donors

`level(id)` returns Stage1/2/3 masks and row colors (same palette as `src/world/LevelDescriptor.cpp`). Unknown ids fall back to Stage1.

`makeState` builds a centered paddle, one **Stuck** ball on the paddle, and bricks only where the mask is `1`:

| Stage | Mask | Brick count |
|-------|------|-------------|
| Stage1 | all 1s | 60 |
| Stage2 | `(row+col)%2==0` | 30 |
| Stage3 | border frame | 28 |

**Donor rule:** live-brick index `i` (mask-1 cells only, row-major, starting at 0). If `i % 4 == 0`, `drop = PowerUpKind((i / 4) % 4)` in enum order Wide, MultiBall, Slow, ExtraLife. Killing a brick with `drop` spawns a capsule at the brick position.

## `step` order

When `cleared` or `over`, `step` returns an empty event list and moves nothing.

Otherwise, with `SimInput { paddleAxis, launch }` and `dt`:

1. **Paddle** — `x += paddleSpeed * axis * dt`, clamp to `[0, designWidth - width]`.
2. **Launch** — first Stuck ball becomes Live with `{160,-360}` (×0.6 if `slowActive`).
3. **Live balls** — `eng::sweep` from current center with `delta = vel * dt` against alive bricks (earliest hit, one brick per ball). On hit: kill, +10 score, `BrickDestroyed`, maybe spawn capsule, `eng::reflect` velocity. Then wall bounce (`x=0`, `x=width-diameter`, `y=0`) and paddle bounce when `vel.y > 0` and overlapping.
4. **Capsules** — fall at 180 px/s; paddle AABB catch → `applyPowerUp` + `PowerUpCaught`; `pos.y >= 720` erases.
5. **Effects** — decrement `remaining`; at ≤0 expire (Wide restores width 120 and clamps; Slow divides live velocities by 0.6 once and clears `slowActive`).
6. **Lives** — if every ball has `pos.y >= 720`: `LifeLost`, decrement lives; if lives remain, one Stuck ball; else `over` + `GameOver`. Partial misses only erase those balls.
7. **Clear** — if bricks non-empty and none alive: set `cleared` and emit `StageCleared` once. An empty brick vector does **not** clear.

Stuck balls stay glued to the paddle each step. A 1200 px/s ball must still destroy a 28 px brick via sweep (no tunneling miss).

## `applyPowerUp`

| Kind | Behavior |
|------|----------|
| Wide | Expire previous timed effect; width 200; timer 8 s |
| Slow | Expire previous; ×0.6 all ball velocities; `slowActive`; timer 8 s |
| ExtraLife | `lives + 1`, no timer |
| MultiBall | Two extras at first ball; Stuck → Live with `{160±120,-360}` (scaled if slow); Live → `vel.x±120` same y |

Wide and Slow share one timer. MultiBall and ExtraLife do not clear it. Expiring Slow must not divide twice (`slowActive` gate).

## Tests

`arkanoid_sim_test` (`tests/arkanoid/sim/`, depends on `arkanoid_sim` only — no SFML) covers paddle clamp, wall bounce, brick score/events, bottom-face reflect, life loss/restick, StageCleared vs empty bricks, stage brick counts, Wide/ExtraLife, missed capsule, Slow expire restore, multiball count 3, and high-speed sweep hit.
