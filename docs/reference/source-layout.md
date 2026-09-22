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
  - src/window/DrawerI.hpp
  - src/window/WindowSFML.hpp
  - src/window/WindowSFML.cpp
  - src/time/ClockI.hpp
  - src/time/ClockSFML.hpp
  - src/time/ClockSFML.cpp
  - src/time/FixedTimestep.hpp
  - src/screen/ScreenI.hpp
  - src/screen/ScreenStack.hpp
  - src/screen/ScreenStack.cpp
  - src/screen/MainMenuScreen.hpp
  - src/screen/MainMenuScreen.cpp
  - src/screen/GameplayScreen.hpp
  - src/screen/GameplayScreen.cpp
  - src/Example.hpp
  - src/Example.cpp
  - CMakeLists.txt
  - cmake/FetchSFML.cmake
  - cmake/FetchGTest.cmake
  - tests/unit_tests/CMakeLists.txt
  - tests/unit_tests/ExampleTest.cpp
  - tests/unit_tests/GameTest.cpp
  - tests/unit_tests/FixedTimestepTest.cpp
  - tests/unit_tests/ScreenStackTest.cpp
  - tests/unit_tests/MenuGameplayTest.cpp
  - tests/mocks/window/WindowMock.hpp
  - tests/mocks/time/ClockMock.hpp
  - tests/unit_tests/fakes/ScreenSpy.hpp
  - .github/workflows/ci.yml
related_docs:
  - ../how-to/build-and-test.md
  - ../../mvp/README.md
  - ../../mvp/10-engine-progress.md
keywords: [layout, directories, gameLib, game, targets, cmake sources, Example, WindowI, ClockI, ScreenI, ScreenStack, MainMenuScreen]
last_reviewed: 2026-09-22
---

# Source layout reference

## Directories

| Path | Responsibility |
|------|----------------|
| `src/main.cpp` | Constructs `WindowSFML`, `ClockSFML`, `ScreenStack` seeded with `MainMenuScreen`, and `Game`, then `run()` |
| `src/Game.hpp` / `Game.cpp` | Process loop: `ClockI::restart`, Closed → `close()`, leftover events to `ScreenStack`, `FixedTimestep` drain unless top `blocksUpdate`, `update(tick)`, `clear` / `screens.draw` / `display`. Holds `WindowI&`, `ClockI&`, `ScreenStack&` |
| `src/window/WindowI.hpp` | Window port (`DrawerI`): `isOpen`, `close`, `pollEvent`, `clear`, `display`, `draw` |
| `src/window/DrawerI.hpp` | Draw seam: `draw(const sf::Drawable&)` |
| `src/window/WindowSFML.hpp` / `WindowSFML.cpp` | Adapter on `sf::RenderWindow`; framerate 60. Compiled into executable `game` only |
| `src/time/ClockI.hpp` | Clock port: `restart`, `getElapsedTime` |
| `src/time/ClockSFML.hpp` / `ClockSFML.cpp` | Adapter on `sf::Clock`. Compiled into executable `game` only |
| `src/time/FixedTimestep.hpp` | Drain helper (`tick` 1/60 s, cap 0.25 s) |
| `src/screen/ScreenI.hpp` | Screen port: `handleEvent`, `update`, `draw(DrawerI&)`, `blocksUpdate`, `blocksDraw` |
| `src/screen/ScreenStack.hpp` / `ScreenStack.cpp` | Ordered screens; deferred `push` / `pop` / `replace`; `top()` (`gameLib`) |
| `src/screen/MainMenuScreen.hpp` / `.cpp` | Enter replaces with `GameplayScreen`; wide bar |
| `src/screen/GameplayScreen.hpp` / `.cpp` | Empty play + `tickCount`; small rectangle |
| `src/Example.hpp` / `Example.cpp` | Windowless helper class in `gameLib` (`add`) |
| `tests/mocks/window/` | `WindowMock` (gmock) |
| `tests/mocks/time/` | `ClockMock` (gmock) |
| `tests/unit_tests/fakes/` | `ScreenSpy` |
| `tests/unit_tests/` | GoogleTest suites (Debug only; no window) |

## Build targets

- **`gameLib`** (STATIC) — `Example.cpp`, `Game.cpp`, `screen/ScreenStack.cpp`, `MainMenuScreen.cpp`, `GameplayScreen.cpp`, listed in `src/CMakeLists.txt`. C++23. Uses SFML **headers/defines** so ports can mention `sf::Event`, `sf::Time`, `sf::Drawable`, `sf::Vector2u`. Does **not** link SFML (no OpenGL in tests).
- **`game`** (executable) — `src/main.cpp`, `src/window/WindowSFML.cpp`, `src/time/ClockSFML.cpp`; links `gameLib` and SFML 3.1 Graphics/Window/System (audio and network are OFF in `cmake/FetchSFML.cmake`).
- Unit tests (Debug only) — GoogleTest 1.18 via `cmake/FetchGTest.cmake`. Suites: `example_test`, `game_test`, `fixed_timestep_test`, `screen_stack_test`, `menu_gameplay_test`. `menu_gameplay_test` also links `SFML::Graphics` (still no window) because the screens construct `sf::RectangleShape`.

## Adding a source file

Any new `.cpp` under `src/` that belongs to the library must be added to the `gameLib` source list in `src/CMakeLists.txt`, otherwise it is never compiled. SFML adapters (`WindowSFML.cpp`, `ClockSFML.cpp`) are listed on the `game` executable, not in `gameLib`. Header-only files (`*.hpp`) do not need listing.
