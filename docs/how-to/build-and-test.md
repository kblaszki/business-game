---
title: Build, test, and format
diataxis: how-to
audience: [ai, human]
related_code:
  - CMakePresets.json
  - CMakeLists.txt
  - tests/unit_tests/CMakeLists.txt
  - tests/unit_tests/fakes/SpyScreen.hpp
  - tests/unit_tests/fakes/DrawerMock.hpp
  - tests/unit_tests/fakes/NullDrawer.hpp
  - src/IDrawer.hpp
  - src/SfmlDrawer.cpp
  - .github/workflows/ci.yml
related_docs:
  - ../tutorials/getting-started.md
  - ../reference/source-layout.md
keywords: [build, test, ctest, format, presets, build_ut, debug, release]
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

Suites (none open a window): `smoke_test` (`DESIGN_SIZE`), `fixed_timestep_test`, `iscreen_dummy_test`, `screen_stack_test`, `screen_transition_test`, `input_mapper_test`, `pause_blocks_ticks_test`, `world_test`, `rect_collision_test`, `arkanoid_session_test`.

Screens and objects draw through `IDrawer`. Production `Game::run` uses `SfmlDrawer` around the window. Unit tests pass `NullDrawer` or `DrawerMock` and never call `sf::RenderTarget::draw`. Test binaries still link SFML Graphics, so headless CI uses Mesa on Windows (`-DSFML_USE_MESA3D=TRUE`) and `xvfb-run` on Linux.

`build_ut` builds every suite registered with `add_unit_test(...)` in `tests/unit_tests/CMakeLists.txt`.

## CI

GitHub Actions (`.github/workflows/ci.yml`) runs the same Debug tests and a Release `game` build on Ubuntu 24.04 and Windows 2022. Linux CI installs SFML Graphics deps including `libfreetype6-dev`, `libharfbuzz-dev`, and `xvfb`, then runs `xvfb-run --auto-servernum ctest`. Windows CI installs MSYS2 `mingw-w64-x86_64-freetype` and `mingw-w64-x86_64-harfbuzz` (SFML 3.1 `find_package(HarfBuzz)` after system FreeType) and configures with `-DSFML_USE_MESA3D=TRUE`. Static FetchContent copies of FreeType/HarfBuzz on MinGW can deadlock at process start when a test links Graphics.

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
