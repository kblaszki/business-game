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
  - games/arkanoid/app/include/arkanoid/app/Scenes.hpp
  - games/arkanoid/app/include/arkanoid/app/MainMenuScene.hpp
  - games/arkanoid/app/include/arkanoid/app/GameplayScene.hpp
  - games/arkanoid/app/include/arkanoid/app/PauseScene.hpp
  - games/arkanoid/app/src/Assets.cpp
  - games/arkanoid/app/src/SceneRender.cpp
  - games/arkanoid/app/src/Bindings.cpp
  - games/arkanoid/app/src/HudModel.cpp
  - games/arkanoid/app/src/Scenes.cpp
  - games/arkanoid/app/src/MainMenuScene.cpp
  - games/arkanoid/app/src/GameplayScene.cpp
  - games/arkanoid/app/src/PauseScene.cpp
  - games/arkanoid/main.cpp
  - tests/arkanoid/app/AssetsTest.cpp
  - tests/arkanoid/app/HudModelTest.cpp
  - tests/arkanoid/app/SceneRenderTest.cpp
  - tests/arkanoid/app/ScenesTest.cpp
related_docs:
  - arkanoid-sim.md
  - engine-core.md
  - engine-input.md
  - engine-resources.md
  - engine-scene.md
  - source-layout.md
  - ../explanation/architecture.md
keywords: [arkanoid, app, Theme, Assets, Bindings, HudModel, textureSpecs, makeHud, Scenes, MainMenuScene, GameplayScene, PauseScene]
last_reviewed: 2026-09-25
---

# Arkanoid app

Presentation helpers in `namespace sgl::arkanoid` for the playable shell. Static library target `arkanoid_app`. No SFML includes — pixel art is `sgl::Image`, input uses `sgl::ActionMap`. Include as `<arkanoid/app/X.hpp>`. Executable target `arkanoid` (`games/arkanoid/main.cpp`) links `arkanoid_app` and `sgl_sfml`, defines `ASSET_DIR` to the repo `assets/` root.

## Assets

`Assets.hpp` / `Assets.cpp` expose procedural textures and the UI font path:

| Symbol | Role |
|--------|------|
| `AssetKeys` | `string_view` keys: `background`, `brick`, `paddle`, `ball`, `capsuleWide` / `Multi` / `Slow` / `Extra` |
| `uiFontRelativePath` | `"fonts/upheavtt.ttf"` (relative to the assets root) |
| `TextureSpec` | `{key, build}` where `build` returns an `sgl::Image` |
| `textureSpecs()` | Span of all eight specs |

Image sizes match the breakout art: background **1280×720** (dark blue with horizontal stripes), brick **110×28**, paddle **120×20**, ball **16×16** (circle with transparent outside), capsules **32×16** (ellipse, transparent outside). Capsule tints: gold / cyan / blue / green with a white highlight band.

`Theme.hpp` holds inline constexpr colors (title gold, HUD text, banner fill, menu button fills/labels) and HUD anchor positions: score `(32,16)`, power `(560,16)`, lives `(1040,16)`. Design size is `designSize` from `sgl::arkanoid::designWidth` / `designHeight`.

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

## Scenes

`AppServices` bundles `{const Actions& actions; const TextureIds& textures; sgl::FontId font}` for scene factories. Factories return `sgl::SceneFactory`:

| Factory | Scene | Traits |
|---------|-------|--------|
| `mainMenu(services)` | `MainMenuScene` | opaque, blocksUpdate, not pausable |
| `gameplay(services, StageId)` | `GameplayScene` | opaque, blocksUpdate, pausable while playing (`!cleared && !over`) |
| `pause(services)` | `PauseScene` | not opaque, blocksUpdate, not pausable |

`MainMenuScene` draws title `"Breakout"` and Start/Quit buttons (`RectCmd` + `TextCmd`, Theme colors). Confirm (or pointer over Start) → `ReplaceScene{gameplay(..., Stage1)}`; cancel (or pointer over Quit) → `QuitApp`.

`GameplayScene` owns `sim::State` from `makeState`. Each update builds `SimInput` from paddle axis + confirm, calls `step`. Stage1/2 `StageCleared` advances via `makeState(next, score, lives)`. Stage3 clear or game over shows HUD banner; confirm restarts the same stage, cancel → `ReplaceScene{mainMenu}`. While playing, pause → `RequestPause`. Exposes `state()` (const and mutable) and `hud()`.

`PauseScene` dims the screen and shows a `"Paused"` panel. Pause/cancel → `PopScene`; confirm → `PopScene` then `ReplaceScene{mainMenu}` (uses `mainMenu()` from `Scenes.hpp`, not `MainMenuScene.hpp`).

`main.cpp` loads every `textureSpecs()` entry and the UI font through `SfmlPlatform::assets()`, builds `AppServices`, constructs `SceneStack` with the pause factory, pushes the main menu, and runs `sgl::App`.
