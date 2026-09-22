---
title: Pause overlay
diataxis: reference
audience: [ai, human]
related_code:
  - src/screen/PauseScreen.hpp
  - src/screen/PauseScreen.cpp
  - src/screen/ScreenI.hpp
  - src/screen/ScreenStack.hpp
  - src/screen/ScreenStack.cpp
  - src/screen/GameplayScreen.cpp
  - src/Game.cpp
  - tests/unit_tests/PauseOverlayTest.cpp
related_docs:
  - input-and-events.md
  - source-layout.md
  - ../../mvp/05-pause.md
  - ../../mvp/10-engine-progress.md
keywords: [pause, overlay, PauseScreen, requestPauseOverlay, FocusLost, blocksUpdate, blocksDraw]
last_reviewed: 2026-09-22
---

# Pause overlay

Facts about the running tree. Prospective extras (World freeze, selectable pause rows, `timeScale`) stay in [`mvp/05-pause.md`](../../mvp/05-pause.md).

## What pause is

`PauseScreen` is a **stack overlay** on `GameplayScreen`. The paused state **is** “`PauseScreen` is top”. It is not a `bool` on gameplay, not a flag on a future `World` / `GameObject`, and not `timeScale = 0`.

| Query | `PauseScreen` | Meaning |
|-------|---------------|---------|
| `blocksUpdate()` | `true` | Stack updates the overlay and **stops**. `GameplayScreen::update` (and `tickCount`) does not run. |
| `blocksDraw()` | `false` | Gameplay still draws underneath a dim rectangle. |

`Game` still polls every frame, including `Closed`. Overlay UI may run `PauseScreen::update` once per frame when the stack reports `blocksUpdate()`; that update is empty today.

A later optional `World::timeScale` (slow-mo) would still **call** `fixedUpdate`. Overlay pause **does not call** the gameplay tick. Do not set `timeScale = 0` to mean pause.

## How it is requested

`ScreenStack::requestPauseOverlay()` is the **only** `push` of `PauseScreen`. `Game` (on `FocusLost`) and `GameplayScreen` (on `Action::Pause`) call that method. They never `push` the overlay ad hoc.

No-op when:

- the stack is empty
- top `isPauseOverlay()`
- top does not `acceptsPauseOverlay()` (menu, spies)
- a pause push is already queued this frame (`pauseQueued`)

`ScreenI` defaults both queries to `false`. `GameplayScreen` accepts; `PauseScreen` is the overlay. `ScreenStack` does not `dynamic_cast` concrete screens.

Same-frame `Action::Pause` + `FocusLost` still yields **one** overlay.

## Resume vs quit

| Input on overlay | Queue | After `draw` |
|------------------|-------|----------------|
| `Action::Pause` or `Cancel` | `pop` | `[GameplayScreen]` |
| `Action::Confirm` | `pop` then `replace(MainMenuScreen)` | `[MainMenuScreen]` |

Quit **must** be that FIFO pair. Only `replace` while the overlay is top would swap the overlay for the menu and leave gameplay underneath. Only `pop` resumes.

`FocusGained` is consumed by `Game` and does **not** resume.

`Action::Pause` on the menu is ignored (`handleAction` → `false`).

## Focus is not an `Action`

`InputMapper::mapEvent(FocusLost)` is `nullopt`. `Game` consumes `FocusLost` / `FocusGained` before the mapper. Remap later must not bind “OS unfocus” to Escape.

## Draw

`Game` clears once. The overlay draws a full-`DESIGN_SIZE` dim `RectangleShape`. It must not `clear` the target. With `blocksDraw == false` the dummy stays visible.

## Tests

`pause_overlay_test` is windowless (links Graphics only because screens hold `RectangleShape`). It asserts frozen `tickCount`, two `draw` calls while paused, resume, quit-to-menu, single enqueue, and `FocusLost` via `Game` + mocks.
