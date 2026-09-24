---
title: Build, test, and format
diataxis: how-to
audience: [ai, human]
related_code:
  - CMakePresets.json
  - CMakeLists.txt
  - tests/unit_tests/CMakeLists.txt
  - .github/workflows/ci.yml
related_docs:
  - ../tutorials/getting-started.md
  - ../reference/source-layout.md
keywords: [build, test, ctest, format, presets, build_ut, debug, release]
last_reviewed: 2026-09-22
---

# Build, test, and format

Prerequisite: toolchain installed (see [getting-started](../tutorials/getting-started.md)). The project is C++23; GoogleTest 1.18 is fetched only for the Debug preset.

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

Tests and GoogleTest exist only when `CMAKE_BUILD_TYPE=Debug`. The suite does not open a window.

```sh
cmake --preset debug
cmake --build --preset debug --target build_ut
ctest --preset debug
```

Suite: `example_test`, `game_test`, `fixed_timestep_test`, `screen_stack_test`, `menu_gameplay_test`, `input_mapper_test`, `pause_overlay_test`, `world_test`, `level_descriptor_test`, `power_up_test`.

`build_ut` builds every suite registered with `add_unit_test(...)` in `tests/unit_tests/CMakeLists.txt`.

## CI

GitHub Actions (`.github/workflows/ci.yml`) runs Debug `build_ut` + `ctest`, then Release `game`, on `ubuntu-24.04` (system g++) and `windows-2022` (MSVC). Tests do not open a window.

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
