---
title: Source layout reference
diataxis: reference
audience: [ai, human]
related_code:
  - src/CMakeLists.txt
  - src/main.cpp
  - src/Game.hpp
  - src/Game.cpp
  - src/window/WindowI.hpp
  - src/window/WindowSFML.hpp
  - src/window/WindowSFML.cpp
  - src/Example.hpp
  - src/Example.cpp
  - CMakeLists.txt
  - cmake/FetchSFML.cmake
  - cmake/FetchGTest.cmake
  - tests/unit_tests/CMakeLists.txt
  - tests/unit_tests/ExampleTest.cpp
  - tests/unit_tests/GameTest.cpp
  - tests/mocks/window/WindowMock.hpp
  - .github/workflows/ci.yml
related_docs:
  - ../how-to/build-and-test.md
  - ../../mvp/README.md
  - ../../mvp/10-engine-progress.md
keywords: [layout, directories, gameLib, game, targets, cmake sources, Example, WindowI, Game]
last_reviewed: 2026-09-22
---

# Source layout reference

## Directories

| Path | Responsibility |
|------|----------------|
| `src/main.cpp` | Constructs `WindowSFML` (`Game::DESIGN_SIZE`, title `"Business game"`) and `Game`, then `run()` |
| `src/Game.hpp` / `Game.cpp` | Process loop: Closed → `close()`, then `clear` / `display`. Holds `WindowI&`, not `sf::RenderWindow` |
| `src/window/WindowI.hpp` | Window port: `isOpen`, `close`, `pollEvent`, `clear`, `display` |
| `src/window/WindowSFML.hpp` / `WindowSFML.cpp` | Adapter on `sf::RenderWindow`; framerate 60. Compiled into executable `game` only |
| `src/Example.hpp` / `Example.cpp` | Windowless helper class in `gameLib` (`add`) |
| `tests/mocks/window/` | `WindowMock` (gmock) |
| `tests/unit_tests/` | GoogleTest suites (Debug only; no window) |

## Build targets

- **`gameLib`** (STATIC) — `Example.cpp`, `Game.cpp`, listed in `src/CMakeLists.txt`. C++23. Uses SFML **headers/defines** so `WindowI` / `Game` can mention `sf::Event` and `sf::Vector2u`. Does **not** link SFML (no OpenGL in tests).
- **`game`** (executable) — `src/main.cpp` and `src/window/WindowSFML.cpp`, links `gameLib` and SFML 3.1 Graphics/Window/System (audio and network are OFF in `cmake/FetchSFML.cmake`).
- Unit tests (Debug only) — GoogleTest 1.18 via `cmake/FetchGTest.cmake`. Suites: `example_test`, `game_test`.

## Adding a source file

Any new `.cpp` under `src/` that belongs to the library must be added to the `gameLib` source list in `src/CMakeLists.txt`, otherwise it is never compiled. The SFML window adapter is listed on the `game` executable, not in `gameLib`. Header-only files (`*.hpp`) do not need listing.
