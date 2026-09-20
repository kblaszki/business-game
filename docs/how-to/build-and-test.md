---
title: Build, test, and format
diataxis: how-to
audience: [ai, human]
related_code:
  - CMakePresets.json
  - CMakeLists.txt
  - tests/unit_tests/CMakeLists.txt
  - tests/unit_tests/fakes/NullRenderTarget.hpp
  - .github/workflows/ci.yml
related_docs:
  - ../tutorials/getting-started.md
  - ../reference/source-layout.md
keywords: [build, test, ctest, format, presets, build_ut, debug, release, mesa]
last_reviewed: 2026-09-20
---

# Build, test, and format

Prerequisite: toolchain installed (see [getting-started](../tutorials/getting-started.md)).

## Configure

```sh
cmake --preset debug     # or: release
```

## Build the game

```sh
cmake --build --preset debug --target game
```

Binary: `build/<preset>/bin/game` (`.exe` on Windows).

## Run tests (Debug only)

Tests and GoogleTest exist only when `CMAKE_BUILD_TYPE=Debug`.

```sh
cmake --preset debug
cmake --build --preset debug --target build_ut
ctest --preset debug
```

Suites (none open a window, none issue OpenGL): `smoke_test` (`DESIGN_SIZE`), `fixed_timestep_test`, `iscreen_dummy_test`, `screen_stack_test`, `screen_transition_test`, `input_mapper_test`, `pause_blocks_ticks_test`, `world_test`, `rect_collision_test`, `arkanoid_session_test`.

`NullRenderTarget` (`tests/unit_tests/fakes/NullRenderTarget.hpp`) overrides `setActive` to return false so `RenderTarget::draw` skips GL. Headless CI has no usable OpenGL context.

`build_ut` builds every suite registered with `add_unit_test(...)` in `tests/unit_tests/CMakeLists.txt`.

## CI

GitHub Actions (`.github/workflows/ci.yml`) runs the same Debug tests and a Release `game` build on Ubuntu 24.04 and Windows 2022. Linux CI installs SFML Graphics deps including `libfreetype6-dev` and `libharfbuzz-dev`. Windows CI installs MSYS2 `mingw-w64-x86_64-freetype` and `mingw-w64-x86_64-harfbuzz` (SFML 3.1 `find_package(HarfBuzz)` after system FreeType). Static FetchContent copies of those two on MinGW can deadlock at process start when a test links Graphics.

Windows CI also copies Mesa 3D software OpenGL DLLs next to `build/debug/bin/` test executables (`GALLIUM_DRIVER=llvmpipe`). Suites that link SFML Graphics import `opengl32.dll` at process start; the runner's display driver hangs there. Mesa must sit beside the `.exe` (Windows loads `System32\opengl32.dll` before `PATH`). This is the same approach SFML uses (`SFML_USE_MESA3D`). Do not install Mesa for a local GPU machine.

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
