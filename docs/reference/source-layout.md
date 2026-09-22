---
title: Source layout reference
diataxis: reference
audience: [ai, human]
related_code:
  - src/CMakeLists.txt
  - src/main.cpp
  - src/Example.hpp
  - src/Example.cpp
  - CMakeLists.txt
  - cmake/FetchSFML.cmake
  - cmake/FetchGTest.cmake
  - tests/unit_tests/CMakeLists.txt
  - tests/unit_tests/ExampleTest.cpp
  - .github/workflows/ci.yml
related_docs:
  - ../how-to/build-and-test.md
  - ../../mvp/README.md
keywords: [layout, directories, gameLib, game, targets, cmake sources, Example]
last_reviewed: 2026-09-22
---

# Source layout reference

## Directories

| Path | Responsibility |
|------|----------------|
| `src/main.cpp` | Entry point; 1280×720 SFML window, event pump, clear/display |
| `src/Example.hpp` / `Example.cpp` | Windowless helper class in `gameLib` (`add`) |
| `tests/unit_tests/` | GoogleTest suites (Debug only; no window) |

## Build targets

- **`gameLib`** (STATIC) — `Example.cpp`, listed in `src/CMakeLists.txt`. C++23. Does **not** link SFML.
- **`game`** (executable) — `src/main.cpp`, links `gameLib` and SFML 3.1 Graphics/Window/System (audio and network are OFF in `cmake/FetchSFML.cmake`).
- Unit tests (Debug only) — GoogleTest 1.18 via `cmake/FetchGTest.cmake`. Suite: `example_test`.

## Adding a source file

Any new `.cpp` under `src/` that belongs to the library must be added to the `gameLib` source list in `src/CMakeLists.txt`, otherwise it is never compiled. Header-only files (`*.hpp`) do not need listing.
