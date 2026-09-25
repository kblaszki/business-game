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
  - games/tetris/app/include/tetris/app/Sounds.hpp
  - games/tetris/app/include/tetris/app/Feedback.hpp
  - games/tetris/app/include/tetris/app/Scenes.hpp
  - games/tetris/app/include/tetris/app/MainMenuScene.hpp
  - games/tetris/app/include/tetris/app/PlayScene.hpp
  - games/tetris/app/include/tetris/app/PauseScene.hpp
  - games/tetris/app/include/tetris/app/GameOverScene.hpp
  - games/tetris/app/src/Bindings.cpp
  - games/tetris/app/src/InputMapping.cpp
  - games/tetris/app/src/BoardRender.cpp
  - games/tetris/app/src/HudModel.cpp
  - games/tetris/app/src/Sounds.cpp
  - games/tetris/app/src/Feedback.cpp
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
  - tests/tetris/app/SoundsTest.cpp
  - tests/tetris/app/FeedbackTest.cpp
related_docs:
  - tetris-sim.md
  - engine-core.md
  - engine-input.md
  - engine-resources.md
  - engine-scene.md
  - engine-audio.md
  - engine-fx.md
  - engine-save.md
  - source-layout.md
  - ../explanation/architecture.md
keywords: [tetris, app, Theme, Bindings, InputMapping, BoardRender, HudModel, Sounds, Feedback, Scenes, PlayScene, GameOverScene]
last_reviewed: 2026-09-25
---

# Tetris app

Presentation shell in `namespace sgl::tetris` for the playable Tetris window. Static library target `tetris_app` links `PUBLIC` `tetris_sim`, `sgl_loop`, `sgl_resources`, `sgl_audio`, `sgl_fx`, and `sgl_save`. No SFML includes — cells are `RectCmd`, input uses `sgl::ActionMap`, playback goes through `AudioI`. Include as `<tetris/app/X.hpp>`. Executable target `tetris` (`games/tetris/main.cpp`) links `tetris_app` and `sgl_sfml`, defines `ASSET_DIR` to the repo `assets/` root, and depends on `check_boundaries`.

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

## Sounds

`soundSpecs()` returns a span of `{key, Pcm}` for **move**, **rotate**, **lock**, **line1**, **line2**, **line3**, **tetris**, **levelUp**, and **gameOver**. Each buffer is built with `sgl::tone`. `SoundIds` holds the uploaded `SoundId` handles. `main.cpp` uploads every spec through `SfmlAudio::upload` (via `platform.audio()`) and stores the ids in `AppServices`.

## Feedback

`Feedback(AudioI&, SoundIds, Pcg32)` maps `TetrisEvent`s to playback and particles (`ParticleSystem` capacity **512**, **12** particles per cleared row). `onEvents` plays one sound per relevant event (`PieceShifted` → move, `PieceRotated` → rotate, `PieceLocked` → lock, `LinesCleared` → line1/2/3 or tetris, `LevelUp`, `GameOver`) and emits one burst per cleared row. `update(Seconds)` and `render(RenderQueue&)` draw on `Layer::Overlay`. `PlayScene` owns `Feedback` and feeds it `game_.step` events each tick.

## High scores

Capacity **5**. Path is `userDataDir("sfml-game-lab") / "tetris.scores"`. `NotFound` or `Corrupt` loads an empty table (corrupt also notes on stderr). `GameOverScene` inserts the final score, shows **"New record"** when it ranks, and saves with `writeTextFileAtomic`; a save failure shows `"Save failed: …"` and is not fatal. Menu and game over both show the current best (`"Best N"` or `"Best --"`).

## Scenes

`AppServices` is `{const Actions& actions; FontId font; uint64_t seed; AudioI& audio; SoundIds sounds; path scoresPath; HighScoreTable& highScores}`. Factories return `sgl::SceneFactory`:

| Factory | Scene | Traits |
|---------|-------|--------|
| `mainMenu(services)` | `MainMenuScene` | opaque, blocksUpdate, not pausable |
| `play(services)` | `PlayScene` | opaque, blocksUpdate, pausable while `!over()` |
| `pause(services)` | `PauseScene` | not opaque, blocksUpdate, not pausable |
| `gameOver(services, score)` | `GameOverScene` | opaque, blocksUpdate, not pausable |

`MainMenuScene` title `"Tetris"`, best score, Start/Quit. Confirm or `pointerPressed` on Start → `ReplaceScene{play}`; cancel or Quit click → `QuitApp`.

`PlayScene` owns `TetrisGame(services.seed)` and `Feedback`. Each update maps input, calls `step`, runs feedback, and on game over `PushScene{gameOver(...)}`. Pause → `RequestPause`. Exposes `game()`, `hud()`, and `feedback()`.

`PauseScene`: pause/cancel → `PopScene`; confirm → `PopScene` then `ReplaceScene{mainMenu}`.

`GameOverScene` shows the final score, best, optional new-record / save-error lines, and Retry/Menu. Confirm or Retry → `PopScene` then `ReplaceScene{play}` (fresh seeded game). Cancel or Menu → `PopScene` then `ReplaceScene{mainMenu}`.

`main.cpp` opens `SfmlPlatform` **1280×720** titled `"Tetris"`, loads the UI font, uploads `soundSpecs()`, loads the high-score table, builds `AppServices` with constant default seed **42** (not from the clock), constructs `SceneStack` with the pause factory, pushes the main menu, and runs `sgl::App`.

## Tests

Headless suites under `tests/tetris/app/`: `InputMappingTest`, `BoardRenderTest`, `HudTest`, `ScenesTest` (including `gameOverRecordsHighScore` with a temp scores path), `SoundsTest`, `FeedbackTest` (`AudioSpy`).
