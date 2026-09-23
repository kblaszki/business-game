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
  - src/input/Action.hpp
  - src/input/InputMapper.hpp
  - src/input/InputMapper.cpp
  - src/screen/ScreenI.hpp
  - src/screen/ScreenStack.hpp
  - src/screen/ScreenStack.cpp
  - src/screen/MainMenuScreen.hpp
  - src/screen/MainMenuScreen.cpp
  - src/screen/GameplayScreen.hpp
  - src/screen/GameplayScreen.cpp
  - src/screen/PauseScreen.hpp
  - src/screen/PauseScreen.cpp
  - src/world/LevelId.hpp
  - src/world/LevelDescriptor.hpp
  - src/world/LevelDescriptor.cpp
  - src/world/GameObject.hpp
  - src/world/GameObject.cpp
  - src/world/World.hpp
  - src/world/World.cpp
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
  - tests/unit_tests/InputMapperTest.cpp
  - tests/unit_tests/PauseOverlayTest.cpp
  - tests/unit_tests/WorldTest.cpp
  - tests/unit_tests/LevelDescriptorTest.cpp
  - tests/mocks/window/WindowMock.hpp
  - tests/mocks/time/ClockMock.hpp
  - tests/unit_tests/fakes/ScreenSpy.hpp
  - .github/workflows/ci.yml
related_docs:
  - input-and-events.md
  - pause-overlay.md
  - world-and-levels.md
  - ../how-to/build-and-test.md
  - ../../mvp/README.md
  - ../../mvp/10-engine-progress.md
keywords: [layout, directories, gameLib, game, targets, cmake sources, Example, WindowI, ClockI, ScreenI, ScreenStack, MainMenuScreen, PauseScreen, InputMapper, Action, World]
last_reviewed: 2026-09-23
---

# Source layout reference

## Directories

| Path | Responsibility |
|------|----------------|
| `src/main.cpp` | Constructs `WindowSFML`, `ClockSFML`, `ScreenStack` seeded with `MainMenuScreen`, and `Game`, then `run()` |
| `src/Game.hpp` / `Game.cpp` | Process loop: Closed → `close()`, `FocusLost` → `requestPauseOverlay`, `FocusGained` no-op, else map → `handleAction` or `handleEvent`; drain unless top `blocksUpdate` |
| `src/window/WindowI.hpp` | Window port (`DrawerI`): `isOpen`, `close`, `pollEvent`, `clear`, `display`, `draw` |
| `src/window/DrawerI.hpp` | Draw seam: `draw(const sf::Drawable&)` |
| `src/window/WindowSFML.hpp` / `WindowSFML.cpp` | Adapter on `sf::RenderWindow`; framerate 60; key repeat off. Compiled into executable `game` only |
| `src/input/Action.hpp` | `enum class Action` (`Confirm`, `Cancel`, `Pause`) |
| `src/input/InputMapper.hpp` / `.cpp` | `KeyPressed` → optional `Action` (`gameLib`) |
| `src/time/ClockI.hpp` | Clock port: `restart`, `getElapsedTime` |
| `src/time/ClockSFML.hpp` / `ClockSFML.cpp` | Adapter on `sf::Clock`. Compiled into executable `game` only |
| `src/time/FixedTimestep.hpp` | Drain helper (`tick` 1/60 s, cap 0.25 s) |
| `src/screen/ScreenI.hpp` | Screen port + `acceptsPauseOverlay` / `isPauseOverlay` (default false) |
| `src/screen/ScreenStack.hpp` / `ScreenStack.cpp` | Stack; `requestPauseOverlay`; `top()`; `handleAction` |
| `src/screen/MainMenuScreen.hpp` / `.cpp` | `Action::Confirm` replaces with `GameplayScreen{Sandbox}` |
| `src/screen/GameplayScreen.hpp` / `.cpp` | Owns `World` from `LevelId`; `update` → `fixedUpdate` |
| `src/world/LevelId.hpp` | `Sandbox` |
| `src/world/LevelDescriptor.hpp` / `.cpp` | `SpawnSpec`, `levelDescriptor`, `makeWorld` |
| `src/world/GameObject.hpp` / `.cpp` | Concrete dummy; move + wrap |
| `src/world/World.hpp` / `.cpp` | Owner + `fixedUpdate` / `draw`; no pause flag |
| `src/screen/PauseScreen.hpp` / `.cpp` | Overlay; `blocksDraw` false; resume / quit-to-menu |
| `src/Example.hpp` / `Example.cpp` | Windowless helper class in `gameLib` (`add`) |
| `tests/mocks/window/` | `WindowMock` (gmock) |
| `tests/mocks/time/` | `ClockMock` (gmock) |
| `tests/unit_tests/fakes/` | `ScreenSpy` |
| `tests/unit_tests/` | GoogleTest suites (Debug only; no window) |

## Build targets

- **`gameLib`** (STATIC) — `Example.cpp`, `Game.cpp`, `input/InputMapper.cpp`, screen sources, `world/GameObject.cpp`, `World.cpp`, `LevelDescriptor.cpp`, listed in `src/CMakeLists.txt`. C++23. Uses SFML **headers/defines**. Does **not** link SFML (no OpenGL in tests).
- **`game`** (executable) — `src/main.cpp`, `src/window/WindowSFML.cpp`, `src/time/ClockSFML.cpp`; links `gameLib` and SFML 3.1 Graphics/Window/System (audio and network are OFF in `cmake/FetchSFML.cmake`).
- Unit tests (Debug only) — GoogleTest 1.18 via `cmake/FetchGTest.cmake`. Suites: `example_test`, `game_test`, `fixed_timestep_test`, `screen_stack_test`, `menu_gameplay_test`, `input_mapper_test`, `pause_overlay_test`, `world_test`, `level_descriptor_test`. Suites that construct screens, `World`, or `GameObject` also link `SFML::Graphics` (still no window) because those types hold `sf::RectangleShape`.

## Adding a source file

Any new `.cpp` under `src/` that belongs to the library must be added to the `gameLib` source list in `src/CMakeLists.txt`, otherwise it is never compiled. SFML adapters (`WindowSFML.cpp`, `ClockSFML.cpp`) are listed on the `game` executable, not in `gameLib`. Header-only files (`*.hpp`) do not need listing.
