---
title: Project direction and status
diataxis: explanation
audience: [ai, human]
related_code:
  - src/main.cpp
  - src/Game.cpp
  - src/ScreenStack.cpp
  - src/MainMenuScreen.cpp
  - src/GameplayScreen.cpp
  - src/PauseScreen.cpp
  - src/World.cpp
related_docs:
  - ../reference/source-layout.md
  - ../../mvp/README.md
keywords: [roadmap, direction, playable slice, SFML, screen stack]
last_reviewed: 2026-09-19
---

# Project direction and status

## Where it is now

A C++23 SFML 3.1 playable slice: CMake presets, FetchContent for SFML (no Audio/Network) and GoogleTest 1.18, and a 1280×720 window that boots `MainMenuScreen`. Enter starts a sandbox `World` with one wrapping dummy; Escape opens `PauseScreen` (world ticks stop); Enter on the overlay returns to the menu. Product domain (board game, business sim, and so on) is not present. The design notes that led here live in [`mvp/`](../../mvp/README.md).

## Where it is going

Product direction is still open. Do not assume board-game, Arkanoid, or other features exist — add them explicitly when asked.

The previous playable prototype lives on the git branch `v0.1-arkanoid` and is not part of this tree.
