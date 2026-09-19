---
title: Source layout reference
diataxis: reference
audience: [ai, human]
related_code:
  - src/CMakeLists.txt
  - src/Game.hpp
  - src/Game.cpp
  - src/main.cpp
  - src/IScreen.hpp
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
  - src/GameObject.hpp
  - src/GameObject.cpp
  - src/makeWorld.hpp
  - src/makeWorld.cpp
  - src/LevelId.hpp
  - src/LevelDescriptor.hpp
  - CMakeLists.txt
  - cmake/FetchSFML.cmake
  - cmake/FetchGTest.cmake
  - tests/unit_tests/CMakeLists.txt
related_docs:
  - ../how-to/build-and-test.md
  - ../../mvp/README.md
keywords: [layout, directories, gameLib, game, targets, cmake sources, ScreenStack]
last_reviewed: 2026-09-19
---

# Source layout reference

## Directories

| Path | Responsibility |
|------|----------------|
| `src/main.cpp` | Entry point; constructs `Game` and calls `run()` |
| `src/Game.hpp` / `Game.cpp` | 1280×720 window, event pump, fixed timestep, owns `ScreenStack` and `InputMapper`; `FocusLost` calls `requestPauseOverlay` (not mapped to `Action::Pause`); `closeRequested` ends `run()` |
| `src/IScreen.hpp` | Screen interface (`handleEvent`, `handleAction`, `update`, `draw`, block flags) |
| `src/ScreenStack.hpp` / `.cpp` | Deferred `requestPush` / `requestPop` / `requestReplace` / `requestPauseOverlay` / `requestClose`; `applyCommands` after draw |
| `src/MainMenuScreen.*` | Boot screen; `Action::Confirm` replaces with sandbox gameplay |
| `src/GameplayScreen.*` | Owns `World`; `Action::Pause` requests the overlay |
| `src/PauseScreen.*` | Overlay: `blocksUpdate`, still draws gameplay; Pause/Cancel pop; Confirm quits to menu |
| `src/Action.hpp` / `src/InputMapper.*` | Enter → Confirm, Escape → Pause; `Action::Cancel` exists but has no key mapping |
| `src/FixedTimestep.hpp` | Accumulator drain helper (tick 1/60 s, clamp 0.25 s) |
| `src/World.*` / `src/GameObject.*` / `src/makeWorld.*` / `src/LevelId.hpp` / `src/LevelDescriptor.hpp` | Sandbox dummy spawn and wrap motion |
| `tests/unit_tests/` | GoogleTest suites (Debug; no window) |

## Build targets

- **`gameLib`** (STATIC) — implementation `.cpp` under `src/`, listed in `src/CMakeLists.txt`. C++23. Links SFML 3 (`SFML::Graphics`, `SFML::System`, `SFML::Window`; Audio and Network modules are not built).
- **`game`** (executable) — only `src/main.cpp`, links `gameLib`.
- **Debug unit tests** — `smoke_test`, `fixed_timestep_test`, `iscreen_dummy_test`, `screen_stack_test`, `screen_transition_test`, `input_mapper_test`, `pause_blocks_ticks_test`, `world_test`.

## Adding a source file

Any new `.cpp` under `src/` must be added to the `gameLib` source list in `src/CMakeLists.txt`, otherwise it is never compiled. Header-only files (`*.hpp`) do not need listing.
