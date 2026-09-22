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
  - reference/input-and-events.md
  - reference/pause-overlay.md
  - explanation/roadmap.md
  - ../mvp/README.md
  - ../mvp/10-engine-progress.md
keywords: [documentation, diataxis, index, map, navigation, docs]
last_reviewed: 2026-09-22
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
| [reference/input-and-events.md](reference/input-and-events.md) | reference | Action, InputMapper, SFML events, handleAction |
| [reference/pause-overlay.md](reference/pause-overlay.md) | reference | PauseScreen, overlay, FocusLost, requestPauseOverlay |
| [explanation/roadmap.md](explanation/roadmap.md) | explanation | roadmap, skeleton, mvp |
| [../mvp/README.md](../mvp/README.md) | design notes (not Diátaxis) | prospective engine plan |
| [../mvp/10-engine-progress.md](../mvp/10-engine-progress.md) | living tracker (not Diátaxis) | WindowI, ScreenStack, InputMapper, PauseScreen, slices |

`mvp/01`–`09` are prospective. Living engine progress is [`mvp/10-engine-progress.md`](../mvp/10-engine-progress.md). Current facts live in the Diátaxis pages above; prefer `src/` over chapter prose if they disagree.

## Maintenance

Documentation currency is enforced by the always-on rule `.cursor/rules/documentation.mdc` and the `update-docs` skill. When code changes, update the docs whose `related_code` lists the touched files, refresh their `last_reviewed`, and keep this table in sync.
