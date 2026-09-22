---
title: Project direction and status
diataxis: explanation
audience: [ai, human]
related_code:
  - src/main.cpp
  - src/Example.cpp
related_docs:
  - ../reference/source-layout.md
  - ../../mvp/README.md
keywords: [roadmap, direction, skeleton, SFML, mvp]
last_reviewed: 2026-09-22
---

# Project direction and status

## Where it is now

A C++23 SFML 3.1 skeleton: CMake presets, FetchContent for SFML (no Audio/Network) and GoogleTest 1.18, a 1280×720 empty window in `src/main.cpp`, and a windowless `Example` type in `gameLib` covered by `example_test`.

Facts: [source-layout.md](../reference/source-layout.md). Historical engine notes: [`mvp/`](../../mvp/README.md).

There is no menu, Arkanoid session, pause overlay, or screen stack in this tree.

## Where it is going

Product direction is still open. Do not assume a board-game or business sim exists — add those explicitly when asked. Use `mvp/` as design notes, not as a description of the current code.
