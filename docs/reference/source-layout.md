---
title: Source layout reference
diataxis: reference
audience: [ai, human]
related_code:
  - src/CMakeLists.txt
  - src/Game.hpp
  - src/Game.cpp
  - src/main.cpp
  - src/window/DrawerI.hpp
  - src/window/WindowI.hpp
  - src/window/WindowSFML.cpp
  - src/SfmlDrawer.hpp
  - src/SfmlDrawer.cpp
  - src/ScreenI.hpp
  - src/ScreenUpdaterI.hpp
  - src/ScreenStack.hpp
  - src/ScreenStack.cpp
  - src/MainMenuScreen.hpp
  - src/MainMenuScreen.cpp
  - src/GameplayScreen.hpp
  - src/GameplayScreen.cpp
  - src/PauseScreen.hpp
  - src/PauseScreen.cpp
  - src/Action.hpp
  - src/InputMapper.hpp
  - src/InputMapper.cpp
  - src/FixedTimestep.hpp
  - src/World.hpp
  - src/World.cpp
  - src/EntityI.hpp
  - src/CollidableI.hpp
  - src/HitTestI.hpp
  - src/entities/Button.hpp
  - src/entities/Button.cpp
  - src/Paddle.hpp
  - src/Paddle.cpp
  - src/Ball.hpp
  - src/Ball.cpp
  - src/Brick.hpp
  - src/Brick.cpp
  - src/RectCollision.hpp
  - src/makeWorld.hpp
  - src/makeWorld.cpp
  - src/LevelId.hpp
  - src/LevelDescriptor.hpp
  - CMakeLists.txt
  - cmake/FetchSFML.cmake
  - cmake/FetchGTest.cmake
  - tests/unit_tests/CMakeLists.txt
  - tests/unit_tests/fakes/SpyScreen.hpp
  - tests/unit_tests/fakes/DrawerMock.hpp
  - tests/unit_tests/fakes/NullDrawer.hpp
  - tests/unit_tests/fakes/WindowMock.hpp
  - .github/workflows/ci.yml
related_docs:
  - application-loop.md
  - screens-and-input.md
  - world-and-levels.md
  - ../how-to/build-and-test.md
  - ../../mvp/README.md
keywords: [layout, directories, gameLib, game, targets, cmake sources, ScreenStack, DrawerI, WindowI]
last_reviewed: 2026-09-20
---

# Source layout reference

## Directories

| Path | Responsibility |
|------|----------------|
| `src/main.cpp` | Entry point; constructs `Game` and calls `run()` |
| `src/Game.hpp` / `Game.cpp` | Window, event pump, `FixedTimestep` — see [application-loop.md](application-loop.md) |
| `src/window/` | `WindowI` facets, `WindowSFML`, `DrawerI` — see [application-loop.md](application-loop.md) |
| `src/ScreenI.hpp` / `src/ScreenUpdaterI.hpp` / `src/ScreenStack.*` / `src/*Screen.*` / `src/Action.hpp` / `src/InputMapper.*` | Screens and consume — see [screens-and-input.md](screens-and-input.md) |
| `src/HitTestI.hpp` / `src/entities/` | Menu `Button` (`EntityI` + `HitTestI`) — see [screens-and-input.md](screens-and-input.md) |
| `src/FixedTimestep.hpp` | Tick 1/60 s, clamp 0.25 s — see [application-loop.md](application-loop.md) |
| `src/World.*` / `src/EntityI.hpp` / `src/CollidableI.hpp` / `src/Paddle.*` / `src/Ball.*` / `src/Brick.*` / `src/RectCollision.hpp` / `src/makeWorld.*` / `src/LevelId.hpp` / `src/LevelDescriptor.hpp` | Arkanoid session — see [world-and-levels.md](world-and-levels.md) |
| `src/window/DrawerI.hpp` / `src/SfmlDrawer.*` | Draw port (`draw(sf::Drawable&)`) and optional `RenderTarget` adapter |
| `tests/unit_tests/` | GoogleTest suites (Debug; loop tests use `WindowMock`, not `WindowSFML`) |
| `tests/unit_tests/fakes/` | `SpyScreen`, `DrawerMock`, `NullDrawer`, `WindowMock` |

## Build targets

- **`gameLib`** (STATIC) — implementation `.cpp` under `src/`, listed in `src/CMakeLists.txt`. C++23. Links SFML 3 (`SFML::Graphics`, `SFML::System`, `SFML::Window`; Audio and Network modules are not built).
- **`game`** (executable) — `src/main.cpp`, links `gameLib`.
- **Debug unit tests** — `smoke_test`, `game_loop_test`, `fixed_timestep_test`, `iscreen_dummy_test`, `button_test`, `screen_stack_test`, `screen_transition_test`, `input_mapper_test`, `pause_blocks_ticks_test`, `world_test`, `rect_collision_test`, `arkanoid_session_test`.

## Adding a source file

Any new `.cpp` under `src/` that belongs to the library must be added to the `gameLib` source list in `src/CMakeLists.txt`, otherwise it is never compiled. Header-only files (`*.hpp`) do not need listing.
