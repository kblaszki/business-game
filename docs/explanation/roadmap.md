---
title: Project direction and status
diataxis: explanation
audience: [ai, human]
related_code:
  - src/entities/Ball.hpp
  - src/entities/Player.hpp
  - src/managers/EntitiesManagerI.hpp
related_docs:
  - ../reference/source-layout.md
  - ./design-decisions.md
keywords: [roadmap, direction, prototype, board game, business, WIP, status]
last_reviewed: 2026-08-01
---

# Project direction and status

## Where it is now

A working SFML prototype: a menu screen with Start/Exit buttons and a game screen with a keyboard-driven paddle. The engine plumbing (window abstraction, event routing, screen switching, entity loop) is the real substance; the gameplay is minimal.

## Where it is going

The longer-term intent is a business / board-style game (working name ideas like *Empire Legends* have been floated). None of that domain exists yet: there is no board, tiles, turns, players-as-domain, or economy. Do not assume board-game features are present — build them explicitly when asked.

## Work in progress and unused code

- `src/entities/Ball.*` — stub files, not in `gameLib`, not production code.
- `src/entities/Player.*` — compiles but no screen uses it.
- `src/managers/EntitiesManagerI.hpp` — a collision/entities-management interface sketch with no implementation.

Treat these as direction hints, not finished building blocks. When you start using or completing one, wire it into the build and update the relevant reference docs plus `docs/index.md`.
