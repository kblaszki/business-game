---
title: Engine scene stack
diataxis: reference
audience: [ai, human]
related_code:
  - engine/scene/include/eng/scene/SceneTraits.hpp
  - engine/scene/include/eng/scene/SceneI.hpp
  - engine/scene/include/eng/scene/SceneRequest.hpp
  - engine/scene/include/eng/scene/SceneContext.hpp
  - engine/scene/include/eng/scene/SceneStack.hpp
  - engine/scene/src/SceneContext.cpp
  - engine/scene/src/SceneStack.cpp
  - tests/engine/scene/SceneStackTest.cpp
  - tests/engine/scene/fakes/SceneSpy.hpp
  - tests/engine/scene/fakes/SceneSpy.cpp
related_docs:
  - source-layout.md
  - engine-input.md
keywords: [eng, SceneI, SceneStack, SceneContext, SceneRequest, SceneTraits, PushScene, PopScene, ReplaceScene, RequestPause, QuitApp]
last_reviewed: 2026-09-24
---

# Engine scene stack

`namespace eng` scene layer. Target `eng_scene` is a STATIC library; PUBLIC link `eng_input`. No SFML. Includes are `<eng/scene/X.hpp>`.

`RenderQueue` is forward-declared only in this module (`SceneI::render` / `SceneStack::render`). Do not include a render header from scene code.

## Types

| Type | Role |
|------|------|
| `SceneTraits` | `opaque` (default true), `blocksUpdate` (default true), `pausable` (default false) |
| `SceneI` | `update(SceneContext&, Seconds)`, `render(RenderQueue&) const`, `traits() const` |
| `SceneFactory` | `std::move_only_function<std::unique_ptr<SceneI>()>` |
| `SceneRequest` | `std::variant` of `PushScene`, `PopScene`, `ReplaceScene`, `RequestPause`, `QuitApp` |
| `SceneContext` | Built by `SceneStack` for a walk; `input()`, `request(SceneRequest)` |
| `SceneStack` | Owns scenes; constructed with a pause-overlay `SceneFactory` |

No concrete game scenes live in `engine/scene/`.

## Update walk

`SceneStack::update(input, dt)`:

1. Clears the per-frame pause latch, then if `input.focusLost()` tries to enqueue `RequestPause` (same rules as an explicit pause).
2. Walks **top to bottom**. Stops after the first scene whose `traits().blocksUpdate` is true.
3. Applies queued requests **after** the walk (never mid-walk).

## Requests

- `PushScene` / `PopScene` / `ReplaceScene` mutate the stack when applied. **Replace on an empty stack acts as Push.**
- `RequestPause` enqueues only when the top scene is `pausable` and a pause was not already queued this frame; apply pushes the pause-overlay factory result.
- `QuitApp` sets `quitRequested()`.
- A second pause in the same update is ignored.

## Render walk

`render` walks from the **highest opaque** scene upward (inclusive). Tests for this module cover update and requests only; they do not call `render` and do not instantiate `RenderQueue`.
