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

A working SFML 3 prototype: a menu screen with Start/Exit buttons and a game screen with a keyboard-driven paddle. Engine plumbing includes window abstraction, typed event routing, a scene stack, resource caching, fixed-timestep updates, and letterboxed resize handling. Gameplay itself is still minimal.

## Where it is going

The longer-term intent is a business / board-style game (working name ideas like *Empire Legends* have been floated). None of that domain exists yet: there is no board, tiles, turns, players-as-domain, or economy. Do not assume board-game features are present — build them explicitly when asked.

## Removed stubs (phase 4)

`Player`, `Ball`, and `EntitiesManagerI` were deleted as unused / unfinished stubs. Reintroduce domain entities when there is a concrete gameplay need (not speculative pong mechanics).

## Refactoring plan

A full architecture/stack audit with a phased refactoring checklist lives in [mvp/refactoring-roadmap.md](../../mvp/refactoring-roadmap.md). Phases 1–5 are complete; further work is feature development on this engine base.
