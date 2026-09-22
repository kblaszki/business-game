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
related_docs:
  - ../reference/source-layout.md
  - ../../mvp/README.md
  - ../../mvp/10-engine-progress.md
keywords: [roadmap, direction, skeleton, SFML, mvp, WindowI]
last_reviewed: 2026-09-22
---

# Project direction and status

## Where it is now

A C++23 SFML 3.1 skeleton with a window port, clock, `ScreenI`, and `ScreenStack`: CMake presets, FetchContent for SFML (no Audio/Network) and GoogleTest 1.18, `Game` driving `WindowI` / `ClockI` / `ScreenStack` (SFML adapters on the `game` executable), and windowless tests.

Facts: [source-layout.md](../reference/source-layout.md). Prospective engine notes: [`mvp/01`–`09`](../../mvp/README.md). What already landed: [`mvp/10-engine-progress.md`](../../mvp/10-engine-progress.md).

There is no menu, Arkanoid session, or pause overlay in this tree.

## Where it is going

Use `mvp/01`–`09` as prospective design notes, not as a description of the current code. Track landed slices in [`mvp/10-engine-progress.md`](../../mvp/10-engine-progress.md).
