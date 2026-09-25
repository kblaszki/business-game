---
title: Build, test, and format
diataxis: how-to
audience: [ai, human]
related_code:
  - CMakePresets.json
  - CMakeLists.txt
  - cmake/FetchSFML.cmake
  - tests/CMakeLists.txt
  - tests/engine/core/CMakeLists.txt
  - tests/arkanoid/sim/CMakeLists.txt
  - tests/arkanoid/app/CMakeLists.txt
  - .github/workflows/ci.yml
related_docs:
  - ../tutorials/getting-started.md
  - ../reference/source-layout.md
  - ../reference/engine-audio.md
keywords: [build, test, ctest, format, presets, build_ut, debug, release, arkanoid, SFML, Audio]
last_reviewed: 2026-09-25
---

# Build, test, and format

Prerequisite: toolchain installed (see [getting-started](../tutorials/getting-started.md)). The project is C++23; GoogleTest 1.18 is fetched only for the Debug preset. Ubuntu CI uses g++-14. CMake FetchContent pulls SFML 3.1 with **Audio** enabled (`SFML_BUILD_AUDIO ON`; Network stays OFF) — no manual SFML install.

## Configure

```sh
cmake --preset debug     # or: release
```

## Build the game

```sh
cmake --build --preset debug --target arkanoid
```

Binary: `build/<preset>/bin/arkanoid` (`.exe` on Windows).

## Run tests (Debug only)

Tests and GoogleTest exist only when `CMAKE_BUILD_TYPE=Debug`. Suites are headless except `event_translate_test` (links `SFML::Window`, opens no window).

```sh
cmake --preset debug
cmake --build --preset debug --target build_ut
ctest --preset debug
```

Suites (`tests/engine/`, `tests/arkanoid/`):

- Engine: `features_test`, `vec_test`, `rect_test`, `handle_test`, `image_test`, `input_state_test`, `scene_stack_test`, `fixed_step_loop_test`, `app_test`, `render_queue_test`, `projection_test`, `collision_test`, `resource_cache_test`, `tone_test`, `event_translate_test`
- Arkanoid: `arkanoid_sim_test`, `arkanoid_assets_test`, `arkanoid_hud_test`, `arkanoid_scene_render_test`, `arkanoid_scenes_test`

`build_ut` builds every suite registered with `add_unit_test(...)` (`cmake/AddUnitTest.cmake`).

## CI

GitHub Actions (`.github/workflows/ci.yml`) runs Debug `build_ut` + `ctest`, then Release `arkanoid`, on `ubuntu-24.04` (g++-14) and `windows-2022` (MSVC).

## Format the code

The `format` target is created only after a Debug configure and runs clang-format in place:

```sh
cmake --build --preset debug --target format
```

## Clean rebuild

If a configure failed halfway (e.g. missing tool cached), delete the preset build dir and reconfigure:

```sh
rm -rf build/debug
cmake --preset debug
```
