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
  - src/window/SfmlDrawer.hpp
  - src/window/SfmlDrawer.cpp
  - src/screens/ScreenI.hpp
  - src/screens/ScreenUpdaterI.hpp
  - src/screens/ScreenStack.hpp
  - src/screens/ScreenStack.cpp
  - src/screens/MainMenuScreen.hpp
  - src/screens/MainMenuScreen.cpp
  - src/screens/GameplayScreen.hpp
  - src/screens/GameplayScreen.cpp
  - src/screens/PauseScreen.hpp
  - src/screens/PauseScreen.cpp
  - src/Action.hpp
  - src/InputMapper.hpp
  - src/InputMapper.cpp
  - src/utils/FixedTimestep.hpp
  - src/World.hpp
  - src/World.cpp
  - src/entities/EntityI.hpp
  - src/entities/CollidableI.hpp
  - src/entities/HitTestI.hpp
  - src/entities/Button.hpp
  - src/entities/Button.cpp
  - src/entities/Paddle.hpp
  - src/entities/Paddle.cpp
  - src/entities/Ball.hpp
  - src/entities/Ball.cpp
  - src/entities/Brick.hpp
  - src/entities/Brick.cpp
  - src/utils/RectCollision.hpp
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
keywords: [layout, directories, gameLib, game, targets, cmake sources, ScreenStack, DrawerI, WindowI, entities, screens, utils]
last_reviewed: 2026-09-20
---

# Source layout reference

## Directories

| Path | Responsibility |
|------|----------------|
| `src/main.cpp` | Entry point; constructs `Game` and calls `run()` |
| `src/Game.hpp` / `Game.cpp` | Window, event pump, `FixedTimestep` — see [application-loop.md](application-loop.md) |
| `src/window/` | `WindowI` facets, `WindowSFML`, `DrawerI`, `SfmlDrawer` — see [application-loop.md](application-loop.md) |
| `src/screens/` | `ScreenI`, `ScreenUpdaterI`, `ScreenStack`, menu / play / pause — see [screens-and-input.md](screens-and-input.md) |
| `src/Action.hpp` / `src/InputMapper.*` | Mapped actions — see [screens-and-input.md](screens-and-input.md) |
| `src/entities/` | `EntityI`, `CollidableI`, `HitTestI`, `Button`, `Paddle`, `Ball`, `Brick` |
| `src/utils/` | `FixedTimestep` (1/60 s, clamp 0.25 s), `RectCollision` |
| `src/World.*` / `src/makeWorld.*` / `src/LevelId.hpp` / `src/LevelDescriptor.hpp` | Arkanoid session — see [world-and-levels.md](world-and-levels.md) |
| `tests/unit_tests/` | GoogleTest suites (Debug; loop tests use `WindowMock`, not `WindowSFML`) |
| `tests/unit_tests/fakes/` | `SpyScreen`, `DrawerMock`, `NullDrawer`, `WindowMock` |

## Build targets

- **`gameLib`** (STATIC) — implementation `.cpp` under `src/`, listed in `src/CMakeLists.txt`. C++23. Links SFML 3 (`SFML::Graphics`, `SFML::System`, `SFML::Window`; Audio and Network modules are not built).
- **`game`** (executable) — `src/main.cpp`, links `gameLib`.
- **Debug unit tests** — `smoke_test`, `game_loop_test`, `fixed_timestep_test`, `iscreen_dummy_test`, `button_test`, `screen_stack_test`, `screen_transition_test`, `input_mapper_test`, `pause_blocks_ticks_test`, `world_test`, `rect_collision_test`, `arkanoid_session_test`.

## Adding a source file

Any new `.cpp` under `src/` that belongs to the library must be added to the `gameLib` source list in `src/CMakeLists.txt`, otherwise it is never compiled. Header-only files (`*.hpp`) do not need listing.
