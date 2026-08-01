---
title: C++ / SFML game development practices
diataxis: explanation
audience: [ai, human]
related_code: []
related_docs:
  - ./from-scratch.md
  - ./bibliography.md
  - ./multithreading.md
  - ./overlay-screens.md
  - ../sfml/index.md
  - ../../index.md
keywords: [game development, SFML, C++, from scratch, best practices, external, multithreading, overlay, pause]
last_reviewed: 2026-08-01
---

# C++ / SFML game development practices

**Repository-agnostic** explanation of how to build a 2D game in C++ with SFML from scratch, drawn from external books, official tutorials, and classic engine articles.

This folder is intentionally **not** a description of any one codebase’s class names or folder layout. Use it as an independent baseline of common practice. Later you can compare a concrete project (including this repository) against the checklist here.

For SFML 3.1 module/API facts that *do* bridge into this repo’s wiring, see [explanation/sfml/](../sfml/index.md).

## Reading order

1. [From scratch with SFML](./from-scratch.md) — bootstrap, loop, input, organization, states, resources, growth path
2. [Overlay screens and pause](./overlay-screens.md) — push/pop pause, update models A/B, input ownership
3. [Multithreading analysis](./multithreading.md) — when threads help, SFML/OS constraints, recommended defaults
4. [Bibliography](./bibliography.md) — primary sources and what each is good for

## How to use this for comparison

Treat [from-scratch.md](./from-scratch.md), [overlay-screens.md](./overlay-screens.md), and [multithreading.md](./multithreading.md) as a checklist:

- Does the project have a clear game loop with a deliberate time model?
- Is input drained every frame; are window lifecycle events handled?
- Are modes (menu / play / pause) explicit?
- Does pause preserve the level via push/pop, and is gameplay input cut under the overlay?
- Are assets owned once and shared?
- Was multithreading introduced only for a measured bottleneck?

Project-specific answers live under [docs/reference/](../../reference/architecture.md) and [docs/explanation/design-decisions.md](../design-decisions.md) — keep those separate from this external baseline.
