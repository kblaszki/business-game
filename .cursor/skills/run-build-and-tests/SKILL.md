---
name: run-build-and-tests
description: >-
  Configures, builds, formats, and runs tests for business-game using CMake presets.
  Use when building the game, running unit tests, formatting code, or verifying CI-like steps.
---

# Run build and tests

Source of truth: [docs/how-to/build-and-test.md](../../../docs/how-to/build-and-test.md) and [docs/tutorials/getting-started.md](../../../docs/tutorials/getting-started.md). Keep this skill consistent with those docs.

## Prerequisites

- CMake ≥ 3.20, Ninja, C++23 compiler
- SFML is fetched by CMake (no manual install)

## Configure

```sh
cmake --preset debug
# or
cmake --preset release
```

Outputs go to `build/debug/` or `build/release/`.

## Build game

```sh
cmake --build --preset debug --target game
# or
cmake --build --preset release --target game
```

Run from the binary directory:

- Windows: `build/debug/bin/game.exe` or `build/release/bin/game.exe`
- Linux/macOS: `build/debug/bin/game` or `build/release/bin/game`

## Tests (Debug only)

Unit tests and GTest are configured only when `CMAKE_BUILD_TYPE` is Debug:

```sh
cmake --preset debug
cmake --build --preset debug --target build_ut
ctest --preset debug
```

## Format

Requires a Debug configure (creates the `format` target):

```sh
cmake --build --preset debug --target format
```

## Typical verify loop after code changes

1. Configure debug (if needed)
2. Build `game` and/or `build_ut`
3. Run `ctest --preset debug`
4. Optionally run `format` before commit
