---
title: Documentation map
diataxis: reference
audience: [ai, human]
related_code:
  - src/main.cpp
  - CMakeLists.txt
related_docs:
  - tutorials/getting-started.md
  - how-to/build-and-test.md
  - reference/source-layout.md
  - reference/application-loop.md
  - reference/screens-and-input.md
  - reference/world-and-levels.md
  - explanation/roadmap.md
  - ../mvp/README.md
keywords: [documentation, diataxis, index, map, navigation, docs]
last_reviewed: 2026-09-20
---

# Documentation map

This directory documents **business-game** following the [Diataxis](https://diataxis.fr/) framework. Every document starts with a YAML front-matter block; agents should search by the `related_code` and `keywords` fields to find the right page.

## How the docs are organized

```mermaid
flowchart TD
  Index[index.md] --> Tut[tutorials/ learning]
  Index --> How[how-to/ tasks]
  Index --> Ref[reference/ facts]
  Index --> Exp[explanation/ rationale]
  Ref -->|related_code| Code[src/ code]
  How -->|related_code| Code
```

| Quadrant | When to read | Purpose |
|----------|--------------|---------|
| Tutorials | First time with the repo | Learn by doing, end to end |
| How-to | You have a specific task | Steps to reach a goal |
| Reference | You need a precise fact | Describe what exists, mirrors code |
| Explanation | You want the "why" | Design rationale and direction |

## All documents

| Document | Type | Keywords |
|----------|------|----------|
| [tutorials/getting-started.md](tutorials/getting-started.md) | tutorial | setup, toolchain, build, run, msys2 |
| [how-to/build-and-test.md](how-to/build-and-test.md) | how-to | build, test, ctest, format, presets |
| [reference/source-layout.md](reference/source-layout.md) | reference | layout, directories, gameLib, targets |
| [reference/application-loop.md](reference/application-loop.md) | reference | Game, FixedTimestep, frame, FocusLost |
| [reference/screens-and-input.md](reference/screens-and-input.md) | reference | IScreen, ScreenStack, Action, Pause |
| [reference/world-and-levels.md](reference/world-and-levels.md) | reference | World, Paddle, Ball, Brick, Arkanoid |
| [explanation/roadmap.md](explanation/roadmap.md) | explanation | roadmap, playable slice, screen stack |
| [../mvp/README.md](../mvp/README.md) | design notes (not Diátaxis) | historical engine plan |

`mvp/` is historical design notes. Current facts live in the Diátaxis pages above; prefer `src/` over chapter prose if they disagree.

## Maintenance

Documentation currency is enforced by the always-on rule `.cursor/rules/documentation.mdc` and the `update-docs` skill. When code changes, update the docs whose `related_code` lists the touched files, refresh their `last_reviewed`, and keep this table in sync.
