---
title: Project direction and status
diataxis: explanation
audience: [ai, human]
related_code:
  - games/arkanoid/main.cpp
  - games/arkanoid/sim/include/arkanoid/sim/State.hpp
  - games/arkanoid/app/include/arkanoid/app/Scenes.hpp
  - engine/scene/include/eng/scene/SceneStack.hpp
  - engine/loop/include/eng/loop/App.hpp
related_docs:
  - ../reference/source-layout.md
  - ../reference/arkanoid-sim.md
  - architecture.md
  - ../../mvp/README.md
  - ../../mvp/10-engine-progress.md
keywords: [roadmap, direction, stages, power-ups, eng, arkanoid, engine split]
last_reviewed: 2026-09-24
---

# Project direction and status

## Where it is now

A C++23 SFML 3.1 breakout on the landed engine split (`engine/` / `games/arkanoid/`): Start/Quit menu, three stages (Stage1 → Stage2 → Stage3), four power-ups (Wide, MultiBall, Slow, ExtraLife), pause overlay via `SceneStack`, headless tests. Legacy `src/` is removed.

Facts: [source-layout.md](../reference/source-layout.md), [arkanoid-sim.md](../reference/arkanoid-sim.md). Contract: [architecture.md](architecture.md). Prospective notes: [`mvp/01`–`09`](../../mvp/README.md). Landed slices: [`mvp/10-engine-progress.md`](../../mvp/10-engine-progress.md).

## Where it is going

Use `mvp/01`–`09` as prospective design notes, not as a description of the current code. Track landed slices in [`mvp/10-engine-progress.md`](../../mvp/10-engine-progress.md).
