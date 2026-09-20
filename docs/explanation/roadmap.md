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
  - src/IDrawer.hpp
related_docs:
  - ../reference/source-layout.md
  - ../reference/application-loop.md
  - ../reference/screens-and-input.md
  - ../reference/world-and-levels.md
  - ../../mvp/README.md
keywords: [roadmap, direction, playable slice, SFML, screen stack]
last_reviewed: 2026-09-20
---

# Project direction and status

## Where it is now

A C++23 SFML 3.1 playable slice: CMake presets, FetchContent for SFML (no Audio/Network) and GoogleTest 1.18, and a 1280×720 window that boots `MainMenuScreen`. Enter starts `LevelId::Arkanoid` (paddle, ball, 50 bricks, 3 lives); Escape opens `PauseScreen` (world ticks stop); Enter on the overlay returns to the menu. Clearing the board or losing the last life also returns to the menu. Screens draw through `IDrawer` (`SfmlDrawer` in `Game::run`; mocks in unit tests).

Facts: [application-loop.md](../reference/application-loop.md), [screens-and-input.md](../reference/screens-and-input.md), [world-and-levels.md](../reference/world-and-levels.md). Historical design notes: [`mvp/`](../../mvp/README.md).

## Where it is going

Product direction is still open. Do not assume a board-game or business sim exists — add those explicitly when asked. The current demo is a simple Arkanoid session on this engine, not a port of the old `v0.1-arkanoid` stack.
