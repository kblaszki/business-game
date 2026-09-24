---
title: Engine input
diataxis: reference
audience: [ai, human]
related_code:
  - engine/input/include/eng/input/Key.hpp
  - engine/input/include/eng/input/MouseButton.hpp
  - engine/input/include/eng/input/InputEvent.hpp
  - engine/input/include/eng/input/ActionId.hpp
  - engine/input/include/eng/input/ActionState.hpp
  - engine/input/include/eng/input/ActionMap.hpp
  - engine/input/include/eng/input/InputState.hpp
  - engine/input/src/ActionMap.cpp
  - engine/input/src/InputState.cpp
  - engine/input/CMakeLists.txt
  - tests/engine/input/InputStateTest.cpp
  - tests/engine/input/CMakeLists.txt
related_docs:
  - source-layout.md
  - engine-core.md
keywords: [eng, eng_input, Key, MouseButton, InputEvent, ActionMap, InputState, ActionId, AxisId, ActionState]
last_reviewed: 2026-09-24
---

# Engine input

SFML-free input types in `namespace eng`, included as `<eng/input/X.hpp>`. Target `eng_input` is a STATIC library that PUBLIC-links `eng_core`.

## Keys and events

`Key` enumerators include letters used by gameplay (`A`, `D`), navigation (`Left`/`Right`/`Up`/`Down`), and UI (`Enter`, `Escape`, `Backspace`, `Space`), plus `Unknown`. `MouseButton` is `Left`, `Right`, `Middle`.

`InputEvent` is a `std::variant` of:

| Alternative | Payload |
|-------------|---------|
| `KeyDown` / `KeyUp` | `Key key` |
| `MouseMove` | `Vec2f pos` |
| `MouseDown` | `MouseButton button`, `Vec2f pos` |
| `WindowClosed` | empty |
| `FocusLost` / `FocusGained` | empty |

## Handles and binding

- `ActionId` = `Handle<ActionTag>`; `AxisId` = `Handle<AxisTag>`
- `ActionMap::bind(key, action)` — one key may map to several actions (small vector per key)
- `ActionMap::bindAxis(negative, positive, axis)` — digital axis from two keys
- `ActionMap::actionsFor(key)` — `std::span<const ActionId>` (empty when unbound)
- `ActionMap::axes()` — span of `{id, negative, positive}` for `InputState` evaluation

## Frame state

`ActionState` has `pressed`, `held`, and `released` edges/levels.

`InputState`:

- `beginFrame()` — clears `pressed`, `released`, `closeRequested`, and `focusLost` only; `held` stays
- `apply(event, map)` — `std::visit` over `InputEvent`
- `action(id)` / `axis(id)` / `pointer()` / `closeRequested()` / `focusLost()`

Semantics:

- Unbound keys leave action state unchanged (still update axis key holds when bound as axis ends)
- `KeyDown` sets `pressed` and `held`; `KeyUp` sets `released` and clears `held`
- Both axis keys held → `0`; only negative → `-1`; only positive → `+1`
- `FocusLost` sets the focus-lost edge and clears all held keys/actions (axes become `0`)
- `WindowClosed` sets `closeRequested` until the next `beginFrame`
- `MouseMove` / `MouseDown` set `pointer()`
- `FocusGained` is a no-op at this layer
