---
title: Engine implementation progress
status: living
last_reviewed: 2026-09-22
related_docs:
  - README.md
  - 09-rollout.md
related_code:
  - ../src/window/WindowI.hpp
  - ../src/window/WindowSFML.hpp
  - ../src/window/WindowSFML.cpp
  - ../src/window/DrawerI.hpp
  - ../src/time/ClockI.hpp
  - ../src/time/ClockSFML.hpp
  - ../src/time/ClockSFML.cpp
  - ../src/time/FixedTimestep.hpp
  - ../src/screen/ScreenI.hpp
  - ../src/screen/ScreenStack.hpp
  - ../src/screen/ScreenStack.cpp
  - ../src/screen/MainMenuScreen.hpp
  - ../src/screen/MainMenuScreen.cpp
  - ../src/screen/GameplayScreen.hpp
  - ../src/screen/GameplayScreen.cpp
  - ../src/screen/PauseScreen.hpp
  - ../src/screen/PauseScreen.cpp
  - ../src/input/Action.hpp
  - ../src/input/InputMapper.hpp
  - ../src/input/InputMapper.cpp
  - ../src/world/LevelId.hpp
  - ../src/world/LevelDescriptor.hpp
  - ../src/world/LevelDescriptor.cpp
  - ../src/world/GameObject.hpp
  - ../src/world/GameObject.cpp
  - ../src/world/World.hpp
  - ../src/world/World.cpp
  - ../src/Game.hpp
  - ../src/Game.cpp
  - ../src/main.cpp
  - ../src/CMakeLists.txt
  - ../tests/mocks/window/WindowMock.hpp
  - ../tests/mocks/time/ClockMock.hpp
  - ../tests/unit_tests/GameTest.cpp
  - ../tests/unit_tests/FixedTimestepTest.cpp
  - ../tests/unit_tests/ScreenStackTest.cpp
  - ../tests/unit_tests/MenuGameplayTest.cpp
  - ../tests/unit_tests/InputMapperTest.cpp
  - ../tests/unit_tests/PauseOverlayTest.cpp
  - ../tests/unit_tests/WorldTest.cpp
  - ../tests/unit_tests/LevelDescriptorTest.cpp
  - ../tests/unit_tests/fakes/ScreenSpy.hpp
---

# Engine implementation progress

This file is the **living register** of names and files that exist in the tree. [`01`](01-architecture.md)–[`09`](09-rollout.md) stay **prospective** (target design, wording “will”). Do not rewrite those chapters into “already implemented.” Facts about running code also belong in [`docs/`](../docs/index.md).

## Conventions in the tree

| Rule | In this tree |
|------|----------------|
| Interfaces | Suffix `FooI` (example: `WindowI`, `ClockI`, `DrawerI`, `ScreenI`) |
| `mvp/` names with prefix `I` | Remap when that type lands — do not edit 01–09 |
| Directories | No `engine/` vs `game/` split ([README.md](README.md) non-goal). Window: `src/window/`. Time: `src/time/`. Screen port: `src/screen/`. Process loop: `Game` in `src/` |
| SOLID | One `WindowI` (inherits `DrawerI` because `ScreenI` must not see `close()`) |
| `gameLib` vs SFML | `gameLib` compiles `Game.cpp`, `InputMapper.cpp`, `ScreenStack.cpp`, and the menu/gameplay screens with **SFML headers only** (no SFML / OpenGL link). `WindowSFML.cpp` and `ClockSFML.cpp` live on executable `game`. `menu_gameplay_test` links Graphics because those screens construct `sf::RectangleShape` |

### Name remap (when those types are added)

| Locked in 01–09 | Name to use in `src/` |
|-----------------|------------------------|
| `IScreen` | `ScreenI` (landed) |
| (other `IFoo` from mvp) | `FooI` |

Until a row is implemented, the mvp spelling in 01–09 is still the design vocabulary for those chapters.

## Slice log

Check a box only after that slice is on `main`. Paths are what landed, not a promise of future files.

| # | Slice | Status | In the tree |
|---|-------|--------|-------------|
| 0 | Scaffold: `Example` in `gameLib`; empty window loop | done (loop moved in slice 1) | `src/Example.*` |
| 1 | Window port | **done** | see slice 1 |
| 2 | Clock + `DrawerI` + `ScreenI` | **done** | see below |
| 3 | `ScreenStack` | **done** | see below |
| 4 | Main menu + gameplay screens | **done** | see below |
| 5 | `InputMapper` + `Action` | **done** | see below |
| 6 | Pause overlay | **done** | see below |
| 7 | World, objects, levels | **done** | see below |

Playable engine slice (menu → dummy → pause → resume / quit) is in the tree. Further work (steering, second level, genre) starts only after names are agreed in chat.

### Slice 1 — window port (landed)

- [x] `WindowI` — `isOpen`, `close`, `pollEvent`, `clear`, `display` (now also `DrawerI`)
- [x] `WindowSFML` — adapter on `sf::RenderWindow`; framerate cap 60
- [x] `Game` — process loop; holds `WindowI&`, **not** `sf::RenderWindow`; `DESIGN_SIZE` `{1280u, 720u}`
- [x] `main.cpp` — wires adapters + `Game`
- [x] `WindowMock` + `GameTest`
- [x] CMake: `Game.cpp` in `gameLib`; `WindowSFML.cpp` on `game`

`pollEvent` returns `std::optional<sf::Event>`. That is a test seam for the window, not a full platform abstraction.

### Slice 2 — clock, `DrawerI`, `ScreenI` (landed)

- [x] `ClockI` / `ClockSFML` — `restart`, `getElapsedTime`; adapter on `game`
- [x] `FixedTimestep` — `tick` 1/60 s, `accumulatorMax` 0.25 s, `drain`
- [x] `DrawerI` — `draw(const sf::Drawable&)`; `WindowI` inherits it
- [x] `ScreenI` — `handleEvent`, `update`, `draw(DrawerI&)`, `blocksUpdate`, `blocksDraw` (no `handleAction`)
- [x] `Game` — Closed in `Game`; leftover events to the (then single) screen; drain unless `blocksUpdate`; `update(tick)` N times; `clear` / `draw` / `display`
- [x] File-local dummy screen in `main.cpp`
- [x] `ClockMock`, `ScreenSpy`, `FixedTimestepTest`, extended `GameTest`

```mermaid
flowchart LR
  Main[main.cpp] --> SFMLWin[WindowSFML]
  Main --> SFMLClk[ClockSFML]
  Main --> Dummy[DummyScreen]
  Main --> Game[Game]
  Game --> WinI[WindowI]
  Game --> ClkI[ClockI]
  Game --> ScrI[ScreenI]
  SFMLWin -.-> WinI
  SFMLClk -.-> ClkI
  Dummy -.-> ScrI
  WinI --> DrwI[DrawerI]
```

| Path | Role |
|------|------|
| [`src/window/WindowI.hpp`](../src/window/WindowI.hpp) | Window port (`DrawerI`) |
| [`src/window/DrawerI.hpp`](../src/window/DrawerI.hpp) | Draw seam |
| [`src/window/WindowSFML.hpp`](../src/window/WindowSFML.hpp) / [`.cpp`](../src/window/WindowSFML.cpp) | SFML window adapter (`game`) |
| [`src/time/ClockI.hpp`](../src/time/ClockI.hpp) | Clock port |
| [`src/time/ClockSFML.hpp`](../src/time/ClockSFML.hpp) / [`.cpp`](../src/time/ClockSFML.cpp) | SFML clock adapter (`game`) |
| [`src/time/FixedTimestep.hpp`](../src/time/FixedTimestep.hpp) | Drain helper |
| [`src/screen/ScreenI.hpp`](../src/screen/ScreenI.hpp) | Screen port |
| [`src/Game.hpp`](../src/Game.hpp) / [`Game.cpp`](../src/Game.cpp) | Loop (`gameLib`) |
| [`src/main.cpp`](../src/main.cpp) | Wires adapters + dummy + `Game` |
| [`tests/mocks/window/WindowMock.hpp`](../tests/mocks/window/WindowMock.hpp) | gmock window |
| [`tests/mocks/time/ClockMock.hpp`](../tests/mocks/time/ClockMock.hpp) | gmock clock |
| [`tests/unit_tests/fakes/ScreenSpy.hpp`](../tests/unit_tests/fakes/ScreenSpy.hpp) | counters |
| [`tests/unit_tests/GameTest.cpp`](../tests/unit_tests/GameTest.cpp) | `game_test` |
| [`tests/unit_tests/FixedTimestepTest.cpp`](../tests/unit_tests/FixedTimestepTest.cpp) | `fixed_timestep_test` |

### Slice 3 — `ScreenStack` (landed)

- [x] `ScreenStack` — `push` / `pop` / `replace`; apply private (immediate when idle, after `draw` when dispatched)
- [x] Walk: events/update top-down; draw from highest `blocksDraw` upward
- [x] `Game(WindowI&, ClockI&, ScreenStack&)`
- [x] Seed in `main` (now `MainMenuScreen`)
- [x] `ScreenStackTest` + `GameTest` on a stack with `ScreenSpy`

```mermaid
flowchart LR
  Main[main.cpp] --> Stack[ScreenStack]
  Main --> Game[Game]
  Game --> Stack
  Stack --> ScrI[ScreenI]
```

| Path | Role |
|------|------|
| [`src/screen/ScreenStack.hpp`](../src/screen/ScreenStack.hpp) / [`.cpp`](../src/screen/ScreenStack.cpp) | Stack (`gameLib`) |
| [`tests/unit_tests/ScreenStackTest.cpp`](../tests/unit_tests/ScreenStackTest.cpp) | `screen_stack_test` |

### Slice 4 — menu and gameplay (landed)

- [x] `MainMenuScreen` — Enter `replace`s with `GameplayScreen`; `blocksUpdate`/`blocksDraw` true; wide bar shape
- [x] `GameplayScreen` — `tickCount`; small dummy rectangle; no World / velocity
- [x] `ScreenStack::top()`
- [x] Dummy removed from `main`
- [x] `menu_gameplay_test`

```mermaid
flowchart LR
  Main[main] --> Menu[MainMenuScreen]
  Menu -->|"Enter replace"| Play[GameplayScreen]
```

| Path | Role |
|------|------|
| [`src/screen/MainMenuScreen.hpp`](../src/screen/MainMenuScreen.hpp) / [`.cpp`](../src/screen/MainMenuScreen.cpp) | Menu |
| [`src/screen/GameplayScreen.hpp`](../src/screen/GameplayScreen.hpp) / [`.cpp`](../src/screen/GameplayScreen.cpp) | Empty play |
| [`tests/unit_tests/MenuGameplayTest.cpp`](../tests/unit_tests/MenuGameplayTest.cpp) | `menu_gameplay_test` |

### Slice 5 — `InputMapper` and `Action` (landed)

- [x] `Action` — `Confirm`, `Cancel` (unbound), `Pause`
- [x] `InputMapper` — `mapEvent` / `mapKeyPressed`; Enter → Confirm; Escape → Pause
- [x] `ScreenI::handleAction` / `ScreenStack::handleAction` (stop on consume only)
- [x] `Game` pump: Closed \| map → handleAction \| handleEvent
- [x] Menu Start = `Confirm`; gameplay ignores `Pause`
- [x] `setKeyRepeatEnabled(false)` on `WindowSFML`
- [x] `input_mapper_test`; no FocusLost, no `pollDummyIntent`

```mermaid
flowchart LR
  Poll[pollEvent] --> Closed{"Closed?"}
  Closed -->|yes| CloseWin[close]
  Closed -->|no| Map[InputMapper]
  Map -->|Action| Act[handleAction]
  Map -->|nullopt| Ev[handleEvent]
```

| Path | Role |
|------|------|
| [`src/input/Action.hpp`](../src/input/Action.hpp) | Enum |
| [`src/input/InputMapper.hpp`](../src/input/InputMapper.hpp) / [`.cpp`](../src/input/InputMapper.cpp) | Table |
| [`tests/unit_tests/InputMapperTest.cpp`](../tests/unit_tests/InputMapperTest.cpp) | `input_mapper_test` |
| [`docs/reference/input-and-events.md`](../docs/reference/input-and-events.md) | Pump + SFML notes |

### Slice 6 — pause overlay (landed)

- [x] `PauseScreen` — `blocksUpdate` true, `blocksDraw` false; dim full-view rect
- [x] `ScreenI::acceptsPauseOverlay` / `isPauseOverlay` (defaults false)
- [x] `ScreenStack::requestPauseOverlay` (only push path; `pauseQueued`)
- [x] Gameplay `Action::Pause` → request; overlay Pause/Cancel → pop; Confirm → pop+replace menu
- [x] `Game`: `FocusLost` → request; `FocusGained` does not resume
- [x] `pause_overlay_test`; freeze = `tickCount` unchanged

```mermaid
flowchart LR
  Play[GameplayScreen] -->|"Pause or FocusLost"| Overlay[PauseScreen]
  Overlay -->|"Pause or Cancel"| Play
  Overlay -->|"Confirm"| Menu[MainMenuScreen]
```

| Path | Role |
|------|------|
| [`src/screen/PauseScreen.hpp`](../src/screen/PauseScreen.hpp) / [`.cpp`](../src/screen/PauseScreen.cpp) | Overlay |
| [`tests/unit_tests/PauseOverlayTest.cpp`](../tests/unit_tests/PauseOverlayTest.cpp) | `pause_overlay_test` |
| [`docs/reference/pause-overlay.md`](../docs/reference/pause-overlay.md) | Overlay facts |

### Slice 7 — World, objects, levels (landed)

- [x] `GameObject` — concrete dummy; `{40,40}`; `{240,0}` px/s; wrap
- [x] `World` — `spawn` / `fixedUpdate` / `draw(DrawerI&)`; no pause flag
- [x] `LevelId::Sandbox` / `levelDescriptor` / `makeWorld`
- [x] `GameplayScreen(ScreenStack&, LevelId)` owns `World`
- [x] Pause freezes pose because `update` is not called
- [x] `world_test`, `level_descriptor_test`

```mermaid
flowchart LR
  Menu[MainMenuScreen] -->|Sandbox| Play[GameplayScreen]
  Play --> World[World]
  World --> Dummy[GameObject]
```

| Path | Role |
|------|------|
| [`src/world/World.hpp`](../src/world/World.hpp) / [`.cpp`](../src/world/World.cpp) | Sim owner |
| [`src/world/GameObject.hpp`](../src/world/GameObject.hpp) / [`.cpp`](../src/world/GameObject.cpp) | Dummy |
| [`src/world/LevelDescriptor.hpp`](../src/world/LevelDescriptor.hpp) / [`.cpp`](../src/world/LevelDescriptor.cpp) | Data + factory |
| [`docs/reference/world-and-levels.md`](../docs/reference/world-and-levels.md) | Facts |
