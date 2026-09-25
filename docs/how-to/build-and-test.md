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
  - .clang-tidy
related_docs:
  - ../tutorials/getting-started.md
  - ../reference/source-layout.md
  - ../reference/engine-audio.md
keywords: [build, test, ctest, format, presets, build_ut, debug, release, asan, coverage, tidy, arkanoid, SFML, Audio]
last_reviewed: 2026-09-25
---

# Build, test, and format

Prerequisite: toolchain installed (see [getting-started](../tutorials/getting-started.md)). The project is C++23; GoogleTest 1.18 is fetched only for Debug-family presets. Ubuntu CI uses g++-14. CMake FetchContent pulls SFML 3.1 with **Audio** enabled (`SFML_BUILD_AUDIO ON`; Network stays OFF) — no manual SFML install.

## Configure

```sh
cmake --preset debug      # or: release, asan, coverage
```

| Preset | Notes |
|--------|--------|
| `debug` | Debug; writes `compile_commands.json` (`CMAKE_EXPORT_COMPILE_COMMANDS ON`) |
| `release` | Release; no unit tests |
| `asan` | Debug + AddressSanitizer and UBSan on GCC/Clang (`SGL_ENABLE_ASAN`; skipped on MSVC) |
| `coverage` | Debug + GCC `--coverage` (`SGL_ENABLE_COVERAGE`) |

Outputs go to `build/<preset>/`.

## Build the game

```sh
cmake --build --preset debug --target arkanoid
```

Binary: `build/<preset>/bin/arkanoid` (`.exe` on Windows).

## Run tests (Debug-family only)

Tests and GoogleTest exist only when `CMAKE_BUILD_TYPE=Debug`. Suites are headless except `event_translate_test` (links `SFML::Window`, opens no window).

```sh
cmake --preset debug
cmake --build --preset debug --target build_ut
ctest --preset debug
```

ASan / coverage (Linux GCC recommended):

```sh
cmake --preset asan
cmake --build --preset asan --target build_ut
ctest --preset asan

cmake --preset coverage
cmake --build --preset coverage --target build_ut
ctest --preset coverage
gcovr --root . --filter 'engine/.*' --exclude 'engine/sfml/.*' --filter 'games/.*/sim/.*' \
  --html-details build/coverage/html/index.html --fail-under-line 90 --object-directory build/coverage
```

Suites (`tests/engine/`, `tests/arkanoid/`, `tests/tetris/`):

- Engine: `vec_test`, `rect_test`, `handle_test`, `image_test`, `random_test`, `input_state_test`, `scene_stack_test`, `fixed_step_loop_test`, `app_test`, `render_queue_test`, `projection_test`, `collision_test`, `resource_cache_test`, `tone_test`, `particle_system_test`, save suites, `event_translate_test`, …
- Arkanoid: `arkanoid_sim_test`, `arkanoid_assets_test`, `arkanoid_hud_test`, `arkanoid_scene_render_test`, `arkanoid_scenes_test`
- Tetris: `tetris_grid_test`, `tetris_srs_test`

`build_ut` builds every suite registered with `add_unit_test(...)` (`cmake/AddUnitTest.cmake`).

## clang-tidy

Requires a Debug configure (for `compile_commands.json`) and `run-clang-tidy` on `PATH`:

```sh
cmake --preset debug
cmake --build --preset debug --target tidy
```

## CI

GitHub Actions (`.github/workflows/ci.yml`):

- `build` — Debug `build_ut` + `ctest`, then Release `arkanoid`, on `ubuntu-24.04` (g++-14) and `windows-2022` (MSVC)
- `asan` — Linux g++-14, preset `asan`, `build_ut` + `ctest`
- `tidy` — Linux clang + blocking `tidy` target over `engine/` and `games/`
- `coverage` — Linux g++-14, preset `coverage`, gcovr with `--fail-under-line 90` filtered to `engine/` (excluding `engine/sfml`) and `games/*/sim`; HTML artifact uploaded

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
