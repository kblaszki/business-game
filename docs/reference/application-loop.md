---
title: Application loop
diataxis: reference
audience: [ai, human]
related_code:
  - src/main.cpp
  - src/Game.hpp
  - src/Game.cpp
  - src/FixedTimestep.hpp
  - src/InputMapper.cpp
  - src/ScreenStack.cpp
  - src/window/DrawerI.hpp
  - src/window/WindowI.hpp
  - src/window/WindowSFML.cpp
  - src/SfmlDrawer.cpp
related_docs:
  - screens-and-input.md
  - world-and-levels.md
  - source-layout.md
keywords: [Game, run, FixedTimestep, frame, FocusLost, pause, applyCommands]
last_reviewed: 2026-09-20
---

# Application loop

`src/main.cpp` constructs `Game` and calls `run()`, which builds a `WindowSFML` and delegates to `run(WindowI&)`. Clock, mapper, and stack are locals inside `run(WindowI&)`, not `Game` members.

The window is `Game::DESIGN_SIZE` (1280×720), title `"Business game"`, framerate limit 60, key repeat off. Boot queues `requestPush(MainMenuScreen)` and calls `applyCommands()` before the first poll. Unit tests call `Game::run(WindowI&)` with `WindowMock` and must not construct `WindowSFML`.

## Frame order

```mermaid
flowchart TD
  Poll[pollEvent]
  Life[handleWindowEvent]
  Map[InputMapper mapEvent]
  Action[handleAction or handleEvent]
  PauseCheck{pauseIsTop}
  Skip[restart clock, no drain]
  Drain[FixedTimestep drain]
  Tick[stack.update tick]
  Draw[clear, stack.draw WindowI]
  Apply[applyCommands]
  Close{closeRequested}
  Display[display]
  Poll --> Life
  Life --> Map
  Map --> Action
  Action --> PauseCheck
  PauseCheck -->|yes| Skip
  PauseCheck -->|no| Drain
  Drain --> Tick
  Skip --> Draw
  Tick --> Draw
  Draw --> Apply
  Apply --> Close
  Close -->|yes| EndNode[window.close]
  Close -->|no| Display
```

1. Poll every `sf::Event`.
2. `handleWindowEvent` consumes lifecycle events (see below). If it returns true, skip mapping.
3. Otherwise `InputMapper::mapEvent`: a mapped `Action` goes to `stack.handleAction` only; an unmapped event goes to `stack.handleEvent` only (never both).
4. If `stack.pauseIsTop()`, `clock.restart()` and **do not** call `drain` or `stack.update`. The frame’s `dt` is discarded so resume does not catch up.
5. Else `FixedTimestep::drain(clock.restart())` and `stack.update(FixedTimestep::tick)` once per returned tick.
6. `window.clear()`, `stack.draw(window)` (`WindowI` is a `DrawerI`), `stack.applyCommands()`.
7. If `stack.closeRequested()`, `window.close()`.
8. `window.display()`.

## Window lifecycle

| Event | Effect |
|-------|--------|
| `Closed` | `WindowCloserI::close()` |
| `Resized` | Consumed; no view / letterbox change |
| `FocusLost` | `stack.requestPauseOverlay()` — not mapped to `Action::Pause` |
| `FocusGained` | Consumed; does not resume |

## Fixed timestep

`FixedTimestep` lives in `Game::run`, not on `World`.

- `tick` = 1/60 s
- `accumulatorMax` = 0.25 s (clamp, then drain)
- Remainder stays in `accumulator` for the next unpaused frame
