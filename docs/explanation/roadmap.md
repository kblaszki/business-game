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
last_reviewed: 2026-09-23
---

# Project direction and status

## Where it is now

A C++23 SFML 3.1 skeleton with window, clock, screens, pause overlay, and a `World` of one wrapping dummy spawned from `LevelId::Sandbox`. CMake presets, FetchContent for SFML (no Audio/Network) and GoogleTest 1.18, windowless tests.

Facts: [source-layout.md](../reference/source-layout.md). Prospective engine notes: [`mvp/01`–`09`](../../mvp/README.md). What already landed: [`mvp/10-engine-progress.md`](../../mvp/10-engine-progress.md).

There is no Arkanoid session, player steering, or second level in this tree.

## Where it is going

Use `mvp/01`–`09` as prospective design notes, not as a description of the current code. Track landed slices in [`mvp/10-engine-progress.md`](../../mvp/10-engine-progress.md).
