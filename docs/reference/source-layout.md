---
title: Source layout reference
diataxis: reference
audience: [ai, human]
related_code:
  - CMakeLists.txt
  - cmake/FetchSFML.cmake
  - cmake/FetchGTest.cmake
  - cmake/AddUnitTest.cmake
  - engine/CMakeLists.txt
  - engine/core/CMakeLists.txt
  - engine/core/include/sgl/core/Features.hpp
  - engine/input/CMakeLists.txt
  - engine/input/include/sgl/input/InputEvent.hpp
  - engine/scene/CMakeLists.txt
  - engine/scene/include/sgl/scene/SceneStack.hpp
  - engine/loop/CMakeLists.txt
  - engine/loop/include/sgl/loop/App.hpp
  - engine/render/CMakeLists.txt
  - engine/collision/CMakeLists.txt
  - engine/resources/CMakeLists.txt
  - engine/resources/include/sgl/resources/ResourceCache.hpp
  - engine/sfml/CMakeLists.txt
  - engine/sfml/include/sgl/sfml/SfmlPlatform.hpp
  - games/CMakeLists.txt
  - games/arkanoid/CMakeLists.txt
  - games/arkanoid/main.cpp
  - games/arkanoid/sim/CMakeLists.txt
  - games/arkanoid/app/CMakeLists.txt
  - games/arkanoid/app/include/arkanoid/app/Scenes.hpp
  - tests/CMakeLists.txt
  - tests/engine/core/CMakeLists.txt
  - tests/engine/input/CMakeLists.txt
  - tests/engine/scene/CMakeLists.txt
  - tests/engine/loop/CMakeLists.txt
  - tests/engine/render/CMakeLists.txt
  - tests/engine/collision/CMakeLists.txt
  - tests/engine/resources/CMakeLists.txt
  - tests/engine/sfml/CMakeLists.txt
  - tests/arkanoid/sim/CMakeLists.txt
  - tests/arkanoid/app/CMakeLists.txt
  - tests/mocks/ClockMock.hpp
  - tests/mocks/PlatformMock.hpp
  - tests/mocks/RendererMock.hpp
  - .github/workflows/ci.yml
related_docs:
  - engine-core.md
  - engine-input.md
  - engine-scene.md
  - engine-loop.md
  - engine-render.md
  - engine-collision.md
  - engine-resources.md
  - engine-sfml.md
  - arkanoid-sim.md
  - arkanoid-app.md
  - input-and-events.md
  - pause-overlay.md
  - ../how-to/build-and-test.md
  - ../explanation/architecture.md
  - ../../mvp/10-engine-progress.md
keywords: [layout, directories, eng, arkanoid, targets, cmake, SceneStack, App, SfmlPlatform]
last_reviewed: 2026-09-25
---

# Source layout reference

Running tree after cutover: `engine/` (`namespace sgl`) and `games/arkanoid/` (`namespace sgl::arkanoid`, executable `arkanoid`). Contract: [architecture.md](../explanation/architecture.md).

## Directories

| Path | Responsibility |
|------|----------------|
| `engine/core/` | Header-only value types (`Vec2`, `Rect`, `Handle`, `Image`, …); target `sgl_core` |
| `engine/input/` | `InputEvent`, `ActionMap`, `InputState`; target `sgl_input` |
| `engine/scene/` | `SceneI`, `SceneStack`, `SceneRequest`, `SceneTraits`; target `sgl_scene` |
| `engine/loop/` | `FixedStepLoop`, `App`, `PlatformI`, `ClockI`; target `sgl_loop` |
| `engine/render/` | `RenderQueue`, `DrawCommand`, `Projection`, `RendererI`; target `sgl_render` |
| `engine/collision/` | AABB / circle / sweep helpers; target `sgl_collision` |
| `engine/resources/` | `ResourceCache`, `ResourceError`; target `sgl_resources` |
| `engine/sfml/` | SFML adapters (`SfmlPlatform`, `EventTranslate`, …); only engine target that links SFML |
| `games/arkanoid/sim/` | Headless breakout rules (`State`, `step`, levels, power-ups); `arkanoid_sim` |
| `games/arkanoid/app/` | Scenes, HUD, assets, bindings; `arkanoid_app` |
| `games/arkanoid/main.cpp` | Wires `SfmlPlatform`, assets, `SceneStack`, `sgl::App`; executable `arkanoid` |
| `assets/fonts/` | UI TTF (`ASSET_DIR` → `assets/`) |
| `tests/engine/` | Per-module engine suites (Debug only) |
| `tests/arkanoid/` | Sim and app suites (Debug only) |
| `tests/mocks/` | `ClockMock`, `PlatformMock`, `RendererMock` (engine ports) |

Include layout: `<sgl/<module>/X.hpp>`, `<arkanoid/<sim|app>/X.hpp>`. Only `engine/sfml` and `games/arkanoid/main.cpp` may include `<SFML/...>`.

## Build targets

| Target | Kind | Notes |
|--------|------|-------|
| `sgl_core` | INTERFACE | Headers only |
| `sgl_input` | STATIC | → `sgl_core` |
| `sgl_scene` | STATIC | → `sgl_input` |
| `sgl_loop` | STATIC | → `sgl_scene`, `sgl_render` |
| `sgl_render` | STATIC | → `sgl_core` |
| `sgl_collision` | STATIC | → `sgl_core` |
| `sgl_resources` | INTERFACE | → `sgl_core` |
| `sgl_sfml` | STATIC | → `sgl_loop`, `sgl_resources`; links SFML Graphics/Window/System |
| `arkanoid_sim` | STATIC | → `sgl_collision`; no SFML |
| `arkanoid_app` | STATIC | → `arkanoid_sim`, `sgl_loop`, `sgl_resources`; no SFML includes |
| `arkanoid` | executable | `games/arkanoid/main.cpp` → `arkanoid_app`, `sgl_sfml`; `ASSET_DIR` → `assets/` |

## Unit test suites (Debug)

Registered with `add_unit_test` under `tests/engine/` and `tests/arkanoid/`:

| Suite | Leaf |
|-------|------|
| `features_test`, `vec_test`, `rect_test`, `handle_test`, `image_test` | `tests/engine/core/` |
| `input_state_test` | `tests/engine/input/` |
| `scene_stack_test` | `tests/engine/scene/` |
| `fixed_step_loop_test`, `app_test` | `tests/engine/loop/` |
| `render_queue_test`, `projection_test` | `tests/engine/render/` |
| `collision_test` | `tests/engine/collision/` |
| `resource_cache_test` | `tests/engine/resources/` |
| `event_translate_test` | `tests/engine/sfml/` (links `SFML::Window`; does not open a window) |
| `arkanoid_sim_test` | `tests/arkanoid/sim/` |
| `arkanoid_assets_test`, `arkanoid_hud_test`, `arkanoid_scene_render_test`, `arkanoid_scenes_test` | `tests/arkanoid/app/` |

All suites are headless except `event_translate_test`, which links SFML Window headers only and still opens no window.

## Adding a source file

New `.cpp` files belong in that module’s own `CMakeLists.txt` (`engine/<module>/CMakeLists.txt` or `games/arkanoid/<sim|app>/CMakeLists.txt`). Header-only files do not need listing. Parallel agents must not edit a shared sources list outside their module.
