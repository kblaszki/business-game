---
title: Project direction and status
diataxis: explanation
audience: [ai, human]
related_code:
  - src/main.cpp
  - src/Game.cpp
  - src/window/WindowI.hpp
  - src/time/ClockI.hpp
  - src/screen/ScreenI.hpp
  - src/screen/ScreenStack.hpp
  - src/screen/MainMenuScreen.hpp
  - src/screen/GameplayScreen.hpp
  - src/input/Action.hpp
  - src/input/InputMapper.hpp
  - src/screen/PauseScreen.hpp
  - src/world/World.hpp
related_docs:
  - ../reference/source-layout.md
  - ../../mvp/README.md
  - ../../mvp/10-engine-progress.md
keywords: [roadmap, direction, skeleton, SFML, mvp, WindowI]
last_reviewed: 2026-09-24
---

# Project direction and status

## Where it is now

A C++23 SFML 3.1 breakout on the engine skeleton: Start/Quit menu, one `Stage1` world (paddle, ball, bricks), pause overlay, windowless tests.

Facts: [source-layout.md](../reference/source-layout.md). Prospective engine notes: [`mvp/01`–`09`](../../mvp/README.md). What already landed: [`mvp/10-engine-progress.md`](../../mvp/10-engine-progress.md).

There is one breakout stage. No power-ups and no second `LevelId`.

## Where it is going

Use `mvp/01`–`09` as prospective design notes, not as a description of the current code. Track landed slices in [`mvp/10-engine-progress.md`](../../mvp/10-engine-progress.md).
