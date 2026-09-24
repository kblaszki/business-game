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
  - src/screen/UiFont.hpp
  - src/screen/UiFont.cpp
  - src/world/LevelId.hpp
  - src/world/LevelDescriptor.hpp
  - src/world/LevelDescriptor.cpp
  - src/world/Paddle.hpp
  - src/world/Ball.hpp
  - src/world/Brick.hpp
  - src/world/BreakoutArt.hpp
  - src/world/World.hpp
  - src/world/World.cpp
  - src/world/PowerUp.hpp
  - src/world/PowerUp.cpp
  - src/Example.hpp
  - src/Example.cpp
  - CMakeLists.txt
  - cmake/FetchSFML.cmake
  - cmake/FetchGTest.cmake
  - cmake/AddUnitTest.cmake
  - engine/core/include/eng/core/Features.hpp
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
  - tests/unit_tests/PowerUpTest.cpp
  - tests/mocks/window/WindowMock.hpp
  - tests/mocks/time/ClockMock.hpp
  - tests/unit_tests/fakes/ScreenSpy.hpp
  - .github/workflows/ci.yml
related_docs:
  - input-and-events.md
  - pause-overlay.md
  - world-and-levels.md
  - power-ups.md
  - ../how-to/build-and-test.md
  - ../../mvp/README.md
  - ../../mvp/10-engine-progress.md
keywords: [layout, directories, gameLib, game, targets, cmake sources, Example, WindowI, ClockI, ScreenI, ScreenStack, MainMenuScreen, PauseScreen, InputMapper, Action, World]
last_reviewed: 2026-09-24
---

# Source layout reference

## Directories

| Path | Responsibility |
|------|----------------|
| `src/main.cpp` | Constructs `WindowSFML`, `ClockSFML`, `ScreenStack` seeded with `MainMenuScreen`, and `Game`, then `run()` |
| `src/Game.hpp` / `Game.cpp` | Process loop: Closed → `close()`, `FocusLost` → `requestPauseOverlay`, `FocusGained` no-op, else map → `handleAction` or `handleEvent`; after the pump, `closeRequested()` → `close()`; drain unless top `blocksUpdate` |
| `src/window/WindowI.hpp` | Window port (`DrawerI`): `isOpen`, `close`, `pollEvent`, `clear`, `display`, `draw` |
| `src/window/DrawerI.hpp` | Draw seam: `draw(const sf::Drawable&)` |
| `src/window/WindowSFML.hpp` / `WindowSFML.cpp` | Adapter on `sf::RenderWindow`; framerate 60; key repeat off. Compiled into executable `game` only |
| `src/input/Action.hpp` | `enum class Action` (`Confirm`, `Cancel`, `Pause`) |
| `src/input/InputMapper.hpp` / `.cpp` | `KeyPressed` → optional `Action` (Enter / Escape / Backspace) |
| `src/time/ClockI.hpp` | Clock port: `restart`, `getElapsedTime` |
| `src/time/ClockSFML.hpp` / `ClockSFML.cpp` | Adapter on `sf::Clock`. Compiled into executable `game` only |
| `src/time/FixedTimestep.hpp` | Drain helper (`tick` 1/60 s, cap 0.25 s) |
| `src/screen/ScreenI.hpp` | Screen port + `acceptsPauseOverlay` / `isPauseOverlay` (default false) |
| `src/screen/ScreenStack.hpp` / `ScreenStack.cpp` | Stack; `requestPauseOverlay`; `requestClose` / `closeRequested`; `top()`; `handleAction` |
| `src/screen/MainMenuScreen.hpp` / `.cpp` | Start/Quit buttons; Confirm → `Stage1`; Cancel → `requestClose` |
| `src/screen/UiFont.hpp` / `.cpp` | `loadUiFont` / `makeUiFont` from `resources/fonts/upheavtt.ttf` (`ASSET_DIR`) |
| `resources/fonts/` | UI TTF files (not FetchContent) |
| `src/screen/GameplayScreen.hpp` / `.cpp` | Owns breakout `World`; paddle keys; Score / Lives / power HUD; Stage1→Stage2→Stage3 advance (score/lives kept); Stage3 You win |
| `src/world/LevelId.hpp` | `Stage1`, `Stage2`, `Stage3` |
| `src/world/LevelDescriptor.hpp` / `.cpp` | Brick grid, row-major mask, `makeWorld` |
| `src/world/Paddle.hpp` / `Ball.hpp` / `Brick.hpp` / `BreakoutArt.hpp` | Breakout pieces + generated textures |
| `src/world/PowerUp.hpp` / `.cpp` | Falling capsule (`PowerUpKind`) |
| `src/world/World.hpp` / `.cpp` | Paddle, many balls, bricks, falling capsules, score, lives, timed Wide/Slow |
| `src/screen/PauseScreen.hpp` / `.cpp` | Overlay; labels; `blocksDraw` false; resume / quit-to-menu |
| `src/Example.hpp` / `Example.cpp` | Windowless helper class in `gameLib` (`add`) |
| `tests/mocks/window/` | `WindowMock` (gmock) |
| `tests/mocks/time/` | `ClockMock` (gmock) |
| `tests/unit_tests/fakes/` | `ScreenSpy` |
| `tests/unit_tests/` | GoogleTest suites (Debug only; no window) |

## Engine split (skeleton)

New engine and game trees are wired into CMake; `src/` `gameLib` / `game` still build until cutover.

| Path | Targets |
|------|---------|
| `engine/core/` | `eng_core` (INTERFACE); `Features.hpp` capability asserts |
| `engine/input/` | `eng_input` (INTERFACE) → `eng_core` |
| `engine/scene/` | `eng_scene` (INTERFACE) → `eng_input` |
| `engine/render/` | `eng_render` (INTERFACE) → `eng_core` |
| `engine/collision/` | `eng_collision` (INTERFACE) → `eng_core` |
| `engine/resources/` | `eng_resources` (INTERFACE) → `eng_core` |
| `engine/loop/` | `eng_loop` (INTERFACE) → `eng_scene`, `eng_render` |
| `engine/sfml/` | `eng_sfml` (STATIC); only target that links SFML |
| `games/arkanoid/sim/` | `arkanoid_sim` (INTERFACE) → `eng_collision` |
| `games/arkanoid/app/` | `arkanoid_app` (INTERFACE) → `arkanoid_sim`, `eng_loop`, `eng_resources` |
| `tests/engine/`, `tests/arkanoid/` | Per-module test leaves; `features_test` under `tests/engine/core/` |

Include layout: `<eng/<module>/X.hpp>` under `engine/<module>/include/`. Only `engine/sfml` and `games/arkanoid/main.cpp` may include SFML.

## Build targets


- **`gameLib`** (STATIC) — `Example.cpp`, `Game.cpp`, `input/InputMapper.cpp`, screens, `world/Paddle.cpp` / `Ball.cpp` / `Brick.cpp` / `BreakoutArt.cpp` / `PowerUp.cpp` / `World.cpp` / `LevelDescriptor.cpp`. C++23. SFML **headers/defines** only. `ASSET_DIR` points at `resources/`.
- **`game`** (executable) — `src/main.cpp`, `src/window/WindowSFML.cpp`, `src/time/ClockSFML.cpp`; links `gameLib` and SFML 3.1 Graphics/Window/System (audio and network are OFF in `cmake/FetchSFML.cmake`).
- Unit tests (Debug only) — GoogleTest 1.18 via `cmake/FetchGTest.cmake`. Suites: `example_test`, `game_test`, `fixed_timestep_test`, `screen_stack_test`, `menu_gameplay_test`, `input_mapper_test`, `pause_overlay_test`, `world_test`, `level_descriptor_test`, `power_up_test`. Suites that construct screens or `World` also link `SFML::Graphics` (still no window).

## Adding a source file

Any new `.cpp` under `src/` that belongs to the library must be added to the `gameLib` source list in `src/CMakeLists.txt`, otherwise it is never compiled. SFML adapters (`WindowSFML.cpp`, `ClockSFML.cpp`) are listed on the `game` executable, not in `gameLib`. Header-only files (`*.hpp`) do not need listing.
