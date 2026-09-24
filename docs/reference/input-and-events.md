---
title: Input and events
diataxis: reference
audience: [ai, human]
related_code:
  - src/input/Action.hpp
  - src/input/InputMapper.hpp
  - src/input/InputMapper.cpp
  - src/Game.cpp
  - src/Game.hpp
  - src/screen/ScreenI.hpp
  - src/screen/ScreenStack.hpp
  - src/screen/ScreenStack.cpp
  - src/screen/MainMenuScreen.cpp
  - src/screen/UiFont.hpp
  - src/screen/UiFont.cpp
  - src/screen/GameplayScreen.cpp
  - src/screen/PauseScreen.cpp
  - src/window/WindowSFML.cpp
  - tests/unit_tests/InputMapperTest.cpp
  - tests/unit_tests/MenuGameplayTest.cpp
  - tests/unit_tests/GameTest.cpp
related_docs:
  - source-layout.md
  - pause-overlay.md
  - ../tutorials/getting-started.md
  - ../../mvp/03-events-and-input.md
  - ../../mvp/10-engine-progress.md
keywords: [input, Action, InputMapper, handleAction, SFML events, KeyPressed, remap, Key, Scancode]
last_reviewed: 2026-09-24
---

# Input and events

Facts about the running tree. Prospective design (hold axes, player remap UI) stays in [`mvp/03-events-and-input.md`](../../mvp/03-events-and-input.md). Overlay pause: [pause-overlay.md](pause-overlay.md).

## SFML 3 event model

`sf::Event` is a **variant**: one subtype is active. Drain with `window.pollEvent()` → `std::optional<sf::Event>`. Inspect with `event->is<T>()` or `event->getIf<T>()`. Do not use `window.waitEvent()`: the loop must keep drawing while simulation is blocked.

`WindowSFML` calls `setKeyRepeatEnabled(false)` so a held key does not spam `KeyPressed`.

### Subtypes (SFML 3.1)

| Subtype | In this tree |
|---------|----------------|
| `Closed` | `Game` calls `window.close()`. Not an `Action`. Not “quit to menu”. |
| `KeyPressed` | `InputMapper::mapEvent` if the key is bound; else `handleEvent` |
| `KeyReleased` | `handleEvent` (mapper returns `nullopt`) |
| `FocusLost` | `Game` calls `requestPauseOverlay()`. Not an `Action`. Not remappable. |
| `FocusGained` | `Game` consumes; does **not** resume. |
| `Resized` | Falls through to `handleEvent`. Letterbox later. Not remappable. |
| `TextEntered` | Ignored. For typing, not gameplay binds. |
| `MouseMoved`, `MouseMovedRaw`, `MouseButtonPressed` / `Released`, `MouseWheelScrolled`, `MouseEntered` / `Left` | `handleEvent`. Not mapped to global `Confirm`. |
| `Joystick*`, `Touch*`, `SensorChanged` | `handleEvent` / ignored. Later bind rows can use joystick buttons. |

Official list: [sf::Event 3.1](https://www.sfml-dev.org/documentation/3.1.0/classsf_1_1Event.html). Tutorial: [Events explained](https://www.sfml-dev.org/tutorials/3.1/window/events/).

### `Key` vs `Scancode`

`KeyPressed` carries both:

- **`code` (`sf::Keyboard::Key`)** — layout label (Enter, Escape, the letter “W” on the current locale). UI binds use this.
- **`scancode` (`sf::Keyboard::Scancode`)** — physical key position. Future movement hold (WASD) should use this so layout changes do not move “W”.

The mapper today reads **`code` only**.

### Edge vs hold

Menus react to **edges** (`KeyPressed` → `Action`). Hold is **not** an `Action`. Continuous axes would be polled (`sf::Keyboard::isKeyPressed` / scancode) from `GameplayScreen::update` later; a pause overlay that blocks `update` then stops hold automatically. Do not mix `isKeyPressed` and `KeyPressed` for the same logical button.

## Pump

Every `pollEvent` result goes into **exactly one** bucket.

```mermaid
flowchart TD
  Poll[Game pollEvent]
  Closed{"Closed?"}
  Focus{"FocusLost or FocusGained?"}
  CloseWin[window.close]
  Overlay[requestPauseOverlay or ignore gain]
  Map[InputMapper mapEvent]
  HasAction{"optional Action?"}
  StackAction[ScreenStack handleAction]
  StackEvent[ScreenStack handleEvent]
  Poll --> Closed
  Closed -->|yes| CloseWin
  Closed -->|no| Focus
  Focus -->|yes| Overlay
  Focus -->|no| Map
  Map --> HasAction
  HasAction -->|yes| StackAction
  HasAction -->|no| StackEvent
```

`InputMapper` is a value member of `Game`. Screens never call `pollEvent` and never `close()` the window. Menu quit is `ScreenStack::requestClose()`; `Game` calls `window.close()` after the event pump if `closeRequested()`.

## `Action` and the binding table

```cpp
enum class Action { Confirm, Cancel, Pause };
```

| Physical input | Result |
|----------------|--------|
| `Enter` | `Confirm` (SFML 3 `Key::Enter` is the labeled Enter/Return; physical numpad Enter is `Scancode::NumpadEnter`, unused here) |
| `Escape` | `Pause` |
| `Backspace` | `Cancel` |
| anything else | `nullopt` → `handleEvent` |

`GameplayScreen::handleAction(Pause)` calls `requestPauseOverlay()`. Overlay `Pause` / `Cancel` resume; `Confirm` quits to menu. Menu Confirm / Start starts `Stage1`. Menu Cancel / Quit calls `requestClose()`. In play, Confirm launches the ball. Clearing Stage1 or Stage2 mid-run loads the next stage (score and lives kept) and does not show You win. Confirm retry is for lose or Stage3 win (same level from scratch); Cancel then returns to menu. Paddle hold is `handleEvent` on Left/Right/A/D, not an `Action`.

`mapEvent` only unwraps `KeyPressed` and delegates to `mapKeyPressed`. Lifecycle events passed in by mistake yield `nullopt`.

## Consume

`handleAction` / `handleEvent` return `true` when the top (or a lower) screen handled the input; the walk stops. `handleAction` walks **top first** and stops **only** on `true`. `blocksUpdate` does **not** cut the action walk (an overlay must still see Escape later).

`handleAction` and `handleEvent` are mutually exclusive **for one SFML event**. If `mapEvent` returns an `Action`, `Game` does not also call `handleEvent`.

Stack `push` / `pop` / `replace` from `handleAction` stay deferred while a walk is in progress; they apply after `draw`, same as `handleEvent`.

## Player remap (later, same seam)

Screens see **intent** (`Action`), not `Key::Enter`. The mapper owns the physical → logical table. A future settings screen would rewrite that table (and persist it). Capture-the-next-key belongs on `handleEvent`, not `handleAction`, so Enter during rebind does not start the game.

Do not remap `Closed`, `Resized`, or focus: those are window/OS, not player binds. Do not map “click anywhere” to `Confirm`; a screen hit-tests its own widgets in `handleEvent`.
