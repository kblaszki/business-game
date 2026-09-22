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
related_docs:
  - ../reference/source-layout.md
  - ../../mvp/README.md
  - ../../mvp/10-engine-progress.md
keywords: [roadmap, direction, skeleton, SFML, mvp, WindowI]
last_reviewed: 2026-09-22
---

# Project direction and status

## Where it is now

A C++23 SFML 3.1 skeleton with window, clock, `ScreenStack`, `InputMapper` / `Action`, `MainMenuScreen`, empty `GameplayScreen`, and `PauseScreen` overlay. CMake presets, FetchContent for SFML (no Audio/Network) and GoogleTest 1.18, windowless tests.

Facts: [source-layout.md](../reference/source-layout.md). Prospective engine notes: [`mvp/01`–`09`](../../mvp/README.md). What already landed: [`mvp/10-engine-progress.md`](../../mvp/10-engine-progress.md).

There is no World, moving dummy, or Arkanoid session in this tree.

## Where it is going

Use `mvp/01`–`09` as prospective design notes, not as a description of the current code. Track landed slices in [`mvp/10-engine-progress.md`](../../mvp/10-engine-progress.md).
