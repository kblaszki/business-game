---
title: Tetris app
diataxis: reference
audience: [ai, human]
related_code:
  - games/tetris/app/include/tetris/app/Theme.hpp
  - games/tetris/app/include/tetris/app/Bindings.hpp
  - games/tetris/app/include/tetris/app/InputMapping.hpp
  - games/tetris/app/include/tetris/app/BoardRender.hpp
  - games/tetris/app/include/tetris/app/HudModel.hpp
  - games/tetris/app/include/tetris/app/Scenes.hpp
  - games/tetris/app/include/tetris/app/MainMenuScene.hpp
  - games/tetris/app/include/tetris/app/PlayScene.hpp
  - games/tetris/app/include/tetris/app/PauseScene.hpp
  - games/tetris/app/include/tetris/app/GameOverScene.hpp
  - games/tetris/app/src/Bindings.cpp
  - games/tetris/app/src/InputMapping.cpp
  - games/tetris/app/src/BoardRender.cpp
  - games/tetris/app/src/HudModel.cpp
  - games/tetris/app/src/Scenes.cpp
  - games/tetris/app/src/MainMenuScene.cpp
  - games/tetris/app/src/PlayScene.cpp
  - games/tetris/app/src/PauseScene.cpp
  - games/tetris/app/src/GameOverScene.cpp
  - games/tetris/main.cpp
  - games/tetris/app/CMakeLists.txt
  - tests/tetris/app/InputMappingTest.cpp
  - tests/tetris/app/BoardRenderTest.cpp
  - tests/tetris/app/HudTest.cpp
  - tests/tetris/app/ScenesTest.cpp
related_docs:
  - tetris-sim.md
  - engine-core.md
  - engine-input.md
  - engine-resources.md
  - engine-scene.md
  - source-layout.md
  - ../explanation/architecture.md
keywords: [tetris, app, Theme, Bindings, InputMapping, BoardRender, HudModel, Scenes, PlayScene, GameOverScene]
last_reviewed: 2026-09-25
---

# Tetris app

Presentation shell in `namespace sgl::tetris` for the playable Tetris window. Static library target `tetris_app` links `PUBLIC` `tetris_sim`, `sgl_loop`, and `sgl_resources`. No SFML includes — cells are `RectCmd`, input uses `sgl::ActionMap`. Include as `<tetris/app/X.hpp>`. Executable target `tetris` (`games/tetris/main.cpp`) links `tetris_app` and `sgl_sfml`, defines `ASSET_DIR` to the repo `assets/` root, and depends on `check_boundaries`.

## Theme

`Theme.hpp` holds design size **1280×720**, cell size **30**, ghost alpha **70**, guideline piece colors, HUD anchors, menu/overlay colors, and `uiFontRelativePath` (`"fonts/upheavtt.ttf"`).

## Bindings

`makeActions()` builds `Actions` with explicit ids **1–10** (`confirm`, `cancel`, `pause`, `left`, `right`, `softDrop`, `hardDrop`, `rotateCw`, `rotateCcw`, `hold`). `defaultBindings` maps:

| Keys | Action |
|------|--------|
| Enter | confirm |
| Backspace | cancel |
| Escape, P | pause |
| Left / Right | left / right |
| Down | softDrop |
| Space | hardDrop |
| Up, X | rotateCw |
| Z | rotateCcw |
| C, LShift | hold |

## Input mapping

`toTetrisInput(const InputState&, const Actions&)` copies `pressed`/`held` edges into `TetrisInput`. Soft drop uses `held`; hard drop, rotates, and hold use `pressed`. A tap that is pressed and released before the next sim step still sets `pressed` because `InputState` keeps the edge until `beginFrame`.

## Board render

`Layout` places a centered well, hold panel (left), and next panel (right). `defaultLayout()` uses cell **30** and preview cell **20**. `renderBoard` draws with **`RectCmd` only**: background, well, locked tiles for the **20** visible rows (`std::views::take(kVisibleHeight)`), ghost (alpha 70) under the active piece on `Layer::Actors`, next queue of **5**, and hold. Buffer rows above 19 are skipped.

## HUD

`makeHud(const Score&)` fills `HudModel` with `"Score N"`, `"Lines N"`, and `"Level N"`. `PlayScene` pushes those strings as `TextCmd`.

## Scenes

`AppServices` is `{const Actions& actions; FontId font; uint64_t seed}`. Factories return `sgl::SceneFactory`:

| Factory | Scene | Traits |
|---------|-------|--------|
| `mainMenu(services)` | `MainMenuScene` | opaque, blocksUpdate, not pausable |
| `play(services)` | `PlayScene` | opaque, blocksUpdate, pausable while `!over()` |
| `pause(services)` | `PauseScene` | not opaque, blocksUpdate, not pausable |
| `gameOver(services, score)` | `GameOverScene` | opaque, blocksUpdate, not pausable |

`MainMenuScene` title `"Tetris"`, Start/Quit. Confirm or `pointerPressed` on Start → `ReplaceScene{play}`; cancel or Quit click → `QuitApp`.

`PlayScene` owns `TetrisGame(services.seed)`. Each update maps input, calls `step`, and on game over `PushScene{gameOver(...)}`. Pause → `RequestPause`. Exposes `game()` and `hud()`.

`PauseScene`: pause/cancel → `PopScene`; confirm → `PopScene` then `ReplaceScene{mainMenu}`.

`GameOverScene` shows the final score with Retry/Menu. Confirm or Retry → `PopScene` then `ReplaceScene{play}` (fresh seeded game). Cancel or Menu → `PopScene` then `ReplaceScene{mainMenu}`.

`main.cpp` opens `SfmlPlatform` **1280×720** titled `"Tetris"`, loads the UI font, builds `AppServices` with constant default seed **42** (not from the clock), constructs `SceneStack` with the pause factory, pushes the main menu, and runs `sgl::App`.

## Tests

Headless suites under `tests/tetris/app/`: `InputMappingTest`, `BoardRenderTest`, `HudTest`, `ScenesTest`.
