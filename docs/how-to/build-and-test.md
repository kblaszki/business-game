---
title: Build, test, and format
diataxis: how-to
audience: [ai, human]
related_code:
  - CMakePresets.json
  - CMakeLists.txt
  - tests/unit_tests/CMakeLists.txt
related_docs:
  - ../tutorials/getting-started.md
  - ../reference/source-layout.md
keywords: [build, test, ctest, format, presets, build_ut, debug, release]
last_reviewed: 2026-08-01
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

Binary: `build/<preset>/bin/game` (`.exe` on Windows). Resources are copied next to it.

## Run tests (Debug only)

Tests and GoogleTest exist only when `CMAKE_BUILD_TYPE=Debug`.

```sh
cmake --preset debug
cmake --build --preset debug --target build_ut
ctest --preset debug
```

Suites: `controllers_test`, `handlers_test`, `managers_test`, `resources_test`, `entities_test`, `screens_test`.

`build_ut` builds every suite registered with `add_unit_test(...)` in `tests/unit_tests/CMakeLists.txt`.

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
