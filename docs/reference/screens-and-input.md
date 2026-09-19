---
title: Screens and input
diataxis: reference
audience: [ai, human]
related_code:
  - src/IScreen.hpp
  - src/ScreenStack.hpp
  - src/ScreenStack.cpp
  - src/MainMenuScreen.cpp
  - src/GameplayScreen.cpp
  - src/PauseScreen.cpp
  - src/Action.hpp
  - src/InputMapper.hpp
  - src/InputMapper.cpp
  - src/Game.cpp
related_docs:
  - application-loop.md
  - world-and-levels.md
  - source-layout.md
keywords: [IScreen, ScreenStack, Action, InputMapper, PauseScreen, consume, overlay]
last_reviewed: 2026-09-19
---

# Screens and input

## IScreen

`handleEvent` and `handleAction` return `bool` (`true` = consume, stop the walk). `update` and `draw` take `sf::Time` / `sf::RenderTarget&` (`draw` is non-const). `blocksUpdate` / `blocksDraw` affect update and draw only — not input.

All three playable screens return `false` from `handleEvent` (keyboard slice; no pointer hit-tests).

## ScreenStack commands

Requests queue; `applyCommands()` applies them FIFO **after** draw. Null push/replace is a no-op.

| Request | Effect at apply |
|---------|-----------------|
| `requestPush` | Append screen |
| `requestPop` | Remove top if any |
| `requestReplace` | Pop top then push (no-op if stack empty) |
| `requestClose` | Sets `closeRequested`; `Game` closes after apply |
| `requestPauseOverlay` | Pushes `PauseScreen` unless already paused, a pause push is already queued, or gameplay is not top |

`gameplayIsTop` / `pauseIsTop` use `dynamic_cast` on `top()`.

## Walks

- **Events and actions:** top → bottom; stop only when a screen returns `true`. `blocksUpdate` is not a cutoff.
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
| `MainMenuScreen` | true / true | `replace(GameplayScreen{LevelId::Sandbox})` | ignored | ignored |
| `GameplayScreen` | true / true | ignored | `requestPauseOverlay()` | ignored |
| `PauseScreen` | true / false | `pop` then `replace(MainMenuScreen)` | `pop` (resume) | `pop` (resume) |

Menu Confirm **replaces** (menu must not stay under play). Pause Confirm must `pop` then `replace`; a single `replace` would swap only the overlay.

The menu does not call `requestClose`. There is no on-screen text or highlighted dual-purpose Confirm row.
