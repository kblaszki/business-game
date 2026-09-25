---
title: Engine loop
diataxis: reference
audience: [ai, human]
related_code:
  - engine/loop/include/sgl/loop/FixedStepLoop.hpp
  - engine/loop/include/sgl/loop/ClockI.hpp
  - engine/loop/include/sgl/loop/PlatformI.hpp
  - engine/loop/include/sgl/loop/App.hpp
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
last_reviewed: 2026-09-25
---

# Engine loop

Platform-independent fixed-timestep pump in `namespace sgl`, included as `<sgl/loop/X.hpp>`. Target `sgl_loop` is a STATIC library that PUBLIC-links `sgl_scene` and `sgl_render`. No SFML dependency.

## FixedStepLoop

`FixedStepLoop(Seconds tick = kTick, Seconds maxFrame = Seconds{0.25f})` accumulates frame deltas in `std::chrono::duration<double>` and returns `StepResult{steps, alpha}`. The stored tick is reconstructed as `1/round(1/tick)` so common rates (60 Hz) stay exact in double despite `Seconds` being `duration<float>`.

- A negative `frame` is treated as zero before clamping.
- Incoming `frame` is clamped to `maxFrame` before accumulating (a 0.5 s hitch yields at most 15 steps at 60 Hz).
- `alpha` is leftover / tick, in `[0, 1)`; leftovers smaller than `1e-6` of a tick are dropped.

## Ports

- `ClockI` — `virtual Seconds restart() = 0`
- `PlatformI` — `isOpen`, `close`, `poll` → `std::optional<InputEvent>`, `renderer()` → `RendererI&`

GMock doubles live at `tests/mocks/ClockMock.hpp`, `tests/mocks/PlatformMock.hpp`, and `tests/mocks/RendererMock.hpp`.

## App

`App(PlatformI&, ClockI&, ActionMap, SceneStack&, RenderQueue&)` owns an `InputState` and a `FixedStepLoop`.

Each `runFrame`:

1. Clear input edges via `input.beginFrame()` only when the previous frame ran at least one update step (`edgesConsumed_`); then drain `platform.poll()` into `input.apply(event, map)`
2. `input.closeRequested()` → `platform.close()`
3. `loop.advance(clock.restart())`; for each step call `stack.update(input, kTick)`, calling `input.beginFrame()` after the first step so action edges are seen once; mark edges consumed when any step ran
4. `stack.quitRequested()` or empty stack → `platform.close()`
5. `queue.clear()`; `stack.render(queue)`; `renderer.begin()` / `submit(queue)` / `end()`
6. store `alpha` for `lastAlpha()`

`pressed` / `released` / `focusLost` (and pointer edges) therefore survive a zero-step frame and remain visible on the first update of the next frame that actually steps.

`run()` loops while `platform.isOpen()`. `runFrames(n)` runs up to `n` frames, checking `isOpen` at the start of each.
