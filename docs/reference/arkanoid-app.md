---
title: Arkanoid app
diataxis: reference
audience: [ai, human]
related_code:
  - games/arkanoid/app/include/arkanoid/app/Theme.hpp
  - games/arkanoid/app/include/arkanoid/app/Assets.hpp
  - games/arkanoid/app/include/arkanoid/app/Bindings.hpp
  - games/arkanoid/app/include/arkanoid/app/HudModel.hpp
  - games/arkanoid/app/include/arkanoid/app/SceneRender.hpp
  - games/arkanoid/app/src/Assets.cpp
  - games/arkanoid/app/src/SceneRender.cpp
  - games/arkanoid/app/src/Bindings.cpp
  - games/arkanoid/app/src/HudModel.cpp
  - tests/arkanoid/app/AssetsTest.cpp
  - tests/arkanoid/app/HudModelTest.cpp
  - tests/arkanoid/app/SceneRenderTest.cpp
related_docs:
  - arkanoid-sim.md
  - engine-core.md
  - engine-input.md
  - engine-resources.md
  - source-layout.md
  - ../explanation/architecture.md
keywords: [arkanoid, app, Theme, Assets, Bindings, HudModel, textureSpecs, makeHud]
last_reviewed: 2026-09-24
---

# Arkanoid app

Presentation helpers in `namespace arkanoid` for the playable shell. Static library target `arkanoid_app`. No SFML includes — pixel art is `eng::Image`, input uses `eng::ActionMap`. Include as `<arkanoid/app/X.hpp>`.

## Assets

`Assets.hpp` / `Assets.cpp` expose procedural textures and the UI font path:

| Symbol | Role |
|--------|------|
| `AssetKeys` | `string_view` keys: `background`, `brick`, `paddle`, `ball`, `capsuleWide` / `Multi` / `Slow` / `Extra` |
| `uiFontRelativePath` | `"fonts/upheavtt.ttf"` (relative to the resources root) |
| `TextureSpec` | `{key, build}` where `build` returns an `eng::Image` |
| `textureSpecs()` | Span of all eight specs |

Image sizes match the legacy breakout art: background **1280×720** (dark blue with horizontal stripes), brick **110×28**, paddle **120×20**, ball **16×16** (circle with transparent outside), capsules **32×16** (ellipse, transparent outside). Capsule tints: gold / cyan / blue / green with a white highlight band — same palette as legacy `PowerUp.cpp`.

`Theme.hpp` holds inline constexpr colors (title gold, HUD text, banner fill, menu button fills/labels) and HUD anchor positions: score `(32,16)`, power `(560,16)`, lives `(1040,16)`. Design size is `designSize` from `arkanoid::designWidth` / `designHeight`.

## Bindings

`makeActions()` builds `Actions` with explicit ids **1–4** (`confirm`, `cancel`, `pause`, `paddle`). `defaultBindings` maps Enter→confirm, Escape→pause, Backspace→cancel, and paddle axis Left/A (negative) with Right/D (positive).

## HUD

`makeHud(const State&)` fills `HudModel`:

| Field | Content |
|-------|---------|
| `score` / `lives` | `"Score N"` / `"Lives N"` |
| `effect` | `"Wide"` or `"Slow"` when `effects.timed` is that kind and `remaining > 0`; else empty |
| `banner` | `"You win"` only when `cleared && stage == Stage3`; `"You lose"` when `over`; otherwise nullopt (including cleared Stage1/2) |
| `hint` | `"Enter - retry   Backspace - menu"` when `banner` is set |

## Render

`renderState` pushes draw commands from a `State` snapshot. Background is `Layer::Background`. Alive bricks are `Layer::World` with depth `box.pos.y`. Paddle, balls, and capsules are `Layer::Actors` with depth equal to their y. Dead bricks are skipped. A wide paddle uses `scale.x = width / 120`. Capsule texture index follows `PowerUpKind`.
