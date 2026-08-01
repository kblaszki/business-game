---
title: SFML 3.1 knowledge base
diataxis: explanation
audience: [ai, human]
related_code:
  - cmake/FetchSFML.cmake
related_docs:
  - ./overview-and-platforms.md
  - ./events.md
  - ./game-architecture.md
  - ./screens-pause-levels.md
  - ../game-dev/index.md
  - ../../index.md
  - ../design-decisions.md
keywords: [SFML, SFML 3.1, knowledge, multimedia, game architecture, index]
last_reviewed: 2026-08-01
---

# SFML 3.1 knowledge base

Conceptual guide to [SFML](https://www.sfml-dev.org/) 3.1 and common patterns for structuring a 2D game on top of it. This is **explanation** material: why APIs look the way they do, and how teams typically compose windows, events, screens, and levels.

It does **not** replace the [official SFML tutorials](https://www.sfml-dev.org/tutorials/3.1/) or [API documentation](https://www.sfml-dev.org/documentation/3.1.0/). For facts about *this* repository’s code, prefer [docs/reference/](../../reference/architecture.md) and the how-to guides.

For **repository-agnostic** from-scratch practices, external bibliography, and a multithreading analysis, see [explanation/game-dev/](../game-dev/index.md).

business-game pins SFML via FetchContent at tag `3.1.0` ([cmake/FetchSFML.cmake](../../../cmake/FetchSFML.cmake)).

## Reading order

1. [Overview and platforms](./overview-and-platforms.md) — modules, platforms, what 3.1 added, what SFML is not
2. [Events](./events.md) — `pollEvent`, type-safe events, real-time input, how this project routes them
3. [Game architecture](./game-architecture.md) — loop, fixed timestep, layering, resources, views
4. [Screens, pause, and levels](./screens-pause-levels.md) — scene stack, overlays, level transitions

## How these pages relate to project docs

| Topic | Concept (here) | Project fact (elsewhere) |
|-------|----------------|--------------------------|
| Event dispatch | [events.md](./events.md) | [event-flow.md](../../reference/event-flow.md) |
| Controllers / loop | [game-architecture.md](./game-architecture.md) | [architecture.md](../../reference/architecture.md) |
| Why interfaces / stack | [game-architecture.md](./game-architecture.md) | [design-decisions.md](../design-decisions.md) |
| Adding a screen | [screens-pause-levels.md](./screens-pause-levels.md) | [add-screen.md](../../how-to/add-screen.md) |

## Official sources

- [SFML 3.1.0 release notes](https://github.com/SFML/SFML/releases/tag/3.1.0)
- [Migrate from SFML 2 to 3](https://www.sfml-dev.org/tutorials/3.1/migration/sfml-2/)
- [Migrate from SFML 3.0 to 3.1](https://www.sfml-dev.org/tutorials/3.1/migration/sfml-3.0/)
- [Events tutorial](https://www.sfml-dev.org/tutorials/3.1/window/events/)
- [FAQ — platforms and modules](https://www.sfml-dev.org/faq/general/)
