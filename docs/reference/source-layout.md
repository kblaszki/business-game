---
title: Source layout reference
diataxis: reference
audience: [ai, human]
related_code:
  - src/CMakeLists.txt
  - src/Game.hpp
  - src/Game.cpp
  - src/main.cpp
  - CMakeLists.txt
  - cmake/FetchSFML.cmake
  - cmake/FetchGTest.cmake
  - tests/unit_tests/CMakeLists.txt
related_docs:
  - ../how-to/build-and-test.md
keywords: [layout, directories, gameLib, game, targets, cmake sources]
last_reviewed: 2026-09-19
---

# Source layout reference

## Directories

| Path | Responsibility |
|------|----------------|
| `src/main.cpp` | Entry point; constructs `Game` and calls `run()` |
| `src/Game.hpp` / `Game.cpp` | Window loop: 1280×720 SFML window, poll `Closed`, clear/display |
| `tests/unit_tests/` | GoogleTest suites (`smoke_test`) |

## Build targets

- **`gameLib`** (STATIC) — implementation `.cpp` under `src/`, listed in `src/CMakeLists.txt`. C++23. Links SFML 3 (`SFML::Graphics`, `SFML::System`, `SFML::Window`; Audio and Network modules are not built).
- **`game`** (executable) — only `src/main.cpp`, links `gameLib`.
- **`smoke_test`** (Debug) — `GameSmokeTest.cpp`; GoogleTest 1.18; asserts `Game::DESIGN_SIZE`.

## Adding a source file

Any new `.cpp` under `src/` must be added to the `gameLib` source list in `src/CMakeLists.txt`, otherwise it is never compiled. Header-only files (`*.hpp`) do not need listing.
