---
title: Screens and input
diataxis: reference
audience: [ai, human]
related_code:
  - src/screens/ScreenI.hpp
  - src/screens/ScreenUpdaterI.hpp
  - src/screens/ScreenStack.hpp
  - src/screens/ScreenStack.cpp
  - src/screens/MainMenuScreen.cpp
  - src/entities/HitTestI.hpp
  - src/entities/Button.hpp
  - src/entities/Button.cpp
  - src/screens/GameplayScreen.cpp
  - src/screens/PauseScreen.cpp
  - src/window/DrawerI.hpp
  - src/window/SfmlDrawer.cpp
  - src/Action.hpp
  - src/InputMapper.hpp
  - src/InputMapper.cpp
  - src/Game.cpp
related_docs:
  - application-loop.md
  - world-and-levels.md
  - source-layout.md
keywords: [ScreenI, ScreenStack, ScreenUpdaterI, Action, InputMapper, PauseScreen, consume, overlay, DrawerI, Button, HitTestI]
last_reviewed: 2026-09-20
---

# Screens and input

## ScreenI

`handleEvent` and `handleAction` return `bool` (`true` = consume, stop the walk). `update` takes `sf::Time`. `draw` takes `DrawerI&` (`draw` is non-const). Production `Game::run(WindowI&)` draws through the window (`WindowI` is a `DrawerI`). Unit tests pass `NullDrawer`, `DrawerMock`, or `WindowMock`. `blocksUpdate` / `blocksDraw` affect update and draw only — not input **except** the event/action walk stops after a screen with `blocksUpdate() == true` (that screen still received the call). Pause overlay therefore cuts paddle keys. `isGameplay` / `isPauseOverlay` default `false`; `GameplayScreen` / `PauseScreen` override them. `gameplayIsTop` / `pauseIsTop` use those flags, not `dynamic_cast`.

Screens hold `ScreenUpdaterI&` (`requestPush` / `Pop` / `Replace` / `Close` / `PauseOverlay`). `ScreenStack` implements `ScreenUpdaterI`.

Pause returns `false` from `handleEvent`. `MainMenuScreen` owns a panel `EntityI` plus start/exit `Button`s (`EntityI` + `HitTestI`, not in `World`). It consumes `MouseMoved` and left `MouseButtonPressed`; hover calls `HitTestI::contains` and brightens the matching button. `GameplayScreen` consumes Left/Right `KeyPressed` / `KeyReleased` to hold the paddle (not `Action`).

## ScreenStack commands

Requests queue; `applyCommands()` applies them FIFO **after** draw. Null push/replace is a no-op.

| Request | Effect at apply |
|---------|-----------------|
| `requestPush` | Append screen |
| `requestPop` | Remove top if any |
| `requestReplace` | Pop top then push (no-op if stack empty) |
| `requestClose` | Sets `closeRequested`; `Game` closes after apply |
| `requestPauseOverlay` | Pushes `PauseScreen` unless already paused, a pause push is already queued, or gameplay is not top |

`gameplayIsTop` / `pauseIsTop` use `isGameplay()` / `isPauseOverlay()` on `top()`. `drawStartIndex()` is the first screen in the draw walk (highest `blocksDraw == true`, or `0`).

## Walks

- **Events and actions:** top → bottom; stop when a screen returns `true` **or** `blocksUpdate() == true` (that screen still ran).
- **Update:** top → bottom; call `update`, then stop after the first `blocksUpdate == true` (that screen still updates).
- **Draw:** find the highest `blocksDraw == true`, then draw that screen through the top (bottom → top of that range). Overlay with `blocksDraw == false` still shows gameplay.

## Action map

`Action` is `Confirm`, `Cancel`, `Pause`. `InputMapper` maps `KeyPressed` only (`KeyReleased` is ignored).

| Key | Action |
|-----|--------|
| Enter | Confirm |
| NumpadEnter (scancode) | Confirm |
| Escape | Pause |
| (none) | Cancel — enum exists, no key |

## Screen transitions

| Screen | blocksUpdate / blocksDraw | Confirm | Pause | Cancel |
|--------|---------------------------|---------|-------|--------|
| `MainMenuScreen` | true / true | `replace(GameplayScreen{LevelId::Arkanoid})` | ignored | ignored |
| `GameplayScreen` | true / true | ignored | `requestPauseOverlay()` | ignored |
| `PauseScreen` | true / false | `pop` then `replace(MainMenuScreen)` | `pop` (resume) | `pop` (resume) |

Clearing all bricks or losing the last life also `requestReplace(MainMenuScreen)`. Left/Right while play is top move the paddle; they are not `Action`s.

Menu Confirm **replaces** (menu must not stay under play). Pause Confirm must `pop` then `replace`; a single `replace` would swap only the overlay.

Clicking start (or Enter) **replaces** with play. Clicking exit calls `requestClose`. There is no on-screen text.
