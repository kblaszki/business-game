---
title: Project direction and status
diataxis: explanation
audience: [ai, human]
related_code:
  - src/main.cpp
  - src/resources/ResourceManager.hpp
related_docs:
  - ../reference/source-layout.md
  - ./design-decisions.md
keywords: [roadmap, direction, prototype, board game, business, status, refactoring]
last_reviewed: 2026-08-01
---

# Project direction and status

## Where it is now

A working SFML 3 prototype: menu with Start/Exit, and an **Arkanoid v1** session in `GameScreen` (paddle, ball, brick grid, 3 lives, win/lose back to menu). Engine plumbing includes window abstraction, typed event routing, a scene stack, resource caching, fixed-timestep updates, and letterboxed resize handling.

## Where it is going

The longer-term intent is a business / board-style game (working name ideas like *Empire Legends* have been floated). None of that domain exists yet: there is no board, tiles, turns, players-as-domain, or economy. Do not assume board-game features are present — build them explicitly when asked. The Arkanoid session is a gameplay exercise on the engine, not the final product.

## Refactoring plan

A full architecture/stack audit with a phased refactoring checklist lives in [mvp/refactoring-roadmap.md](../../mvp/refactoring-roadmap.md). Phases 1–5 are complete.
