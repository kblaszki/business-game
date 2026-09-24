---
title: Engine loop
diataxis: reference
audience: [ai, human]
related_code:
  - engine/loop/include/eng/loop/FixedStepLoop.hpp
  - engine/loop/include/eng/loop/ClockI.hpp
  - engine/loop/include/eng/loop/PlatformI.hpp
  - engine/loop/include/eng/loop/App.hpp
  - engine/loop/src/FixedStepLoop.cpp
  - engine/loop/src/App.cpp
  - tests/engine/loop/FixedStepLoopTest.cpp
  - tests/engine/loop/AppTest.cpp
  - tests/mocks/ClockMock.hpp
  - tests/mocks/PlatformMock.hpp
related_docs:
  - source-layout.md
  - engine-core.md
  - engine-input.md
  - engine-scene.md
  - engine-render.md
keywords: [eng, FixedStepLoop, StepResult, ClockI, PlatformI, App, alpha, kTick, runFrames]
last_reviewed: 2026-09-24
---

# Engine loop

Platform-independent fixed-timestep pump in `namespace eng`, included as `<eng/loop/X.hpp>`. Target `eng_loop` is a STATIC library that PUBLIC-links `eng_scene` and `eng_render`. No SFML dependency.

## FixedStepLoop

`FixedStepLoop(Seconds tick = kTick, Seconds maxFrame = Seconds{0.25f})` accumulates frame deltas and returns `StepResult{steps, alpha}`.

- Incoming `frame` is clamped to `maxFrame` before accumulating (a 0.5 s hitch yields at most 15 steps at 60 Hz).
- `alpha` is leftover / tick, in `[0, 1)`.

## Ports

- `ClockI` — `virtual Seconds restart() = 0`
- `PlatformI` — `isOpen`, `close`, `poll` → `std::optional<InputEvent>`, `renderer()` → `RendererI&`

GMock doubles live at `tests/mocks/ClockMock.hpp`, `tests/mocks/PlatformMock.hpp`, and `tests/mocks/RendererMock.hpp`.

## App

`App(PlatformI&, ClockI&, ActionMap, SceneStack&, RenderQueue&)` owns an `InputState` and a `FixedStepLoop`.

Each `runFrame`:

1. `input.beginFrame()` then drain `platform.poll()` into `input.apply(event, map)`
2. `input.closeRequested()` → `platform.close()`
3. `loop.advance(clock.restart())`; for each step call `stack.update(input, kTick)`, calling `input.beginFrame()` after the first step so action edges are seen once
4. `stack.quitRequested()` → `platform.close()`
5. `queue.clear()`; `stack.render(queue)`; `renderer.begin()` / `submit(queue)` / `end()`
6. store `alpha` for `lastAlpha()`

`run()` loops while `platform.isOpen()`. `runFrames(n)` runs up to `n` frames, checking `isOpen` at the start of each.
