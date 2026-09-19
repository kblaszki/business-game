---
title: Getting started
diataxis: tutorial
audience: [ai, human]
related_code:
  - CMakePresets.json
  - CMakeLists.txt
  - cmake/FetchSFML.cmake
  - cmake/FetchGTest.cmake
  - src/Game.cpp
related_docs:
  - ../how-to/build-and-test.md
  - ../reference/source-layout.md
  - ../reference/application-loop.md
  - ../reference/screens-and-input.md
  - ../reference/world-and-levels.md
keywords: [setup, toolchain, build, run, msys2, ninja, cmake, first time]
last_reviewed: 2026-09-19
---

# Getting started

Goal: from a fresh clone to the playable slice. By the end you will have configured, built, and launched `game`: menu → Arkanoid → pause overlay.

## 1. Install the toolchain

You need CMake, Ninja, and a C++23 compiler on your `PATH`. On Windows this repo is set up around MSYS2 MinGW.

MSYS2 MinGW 64-bit shell:

```bash
pacman -S --needed \
  mingw-w64-x86_64-gcc \
  mingw-w64-x86_64-ninja \
  mingw-w64-x86_64-cmake
```

Add `C:\msys64\mingw64\bin` to `PATH` and restart the terminal. Verify:

```sh
g++ --version
ninja --version
cmake --version
```

SFML 3.1 (Graphics/Window/System only) and GoogleTest 1.18 are fetched automatically by CMake (`cmake/FetchSFML.cmake`, `cmake/FetchGTest.cmake`). No separate SFML install. On Ubuntu, install `libfreetype6-dev` and `libharfbuzz-dev` (and the usual X11/OpenGL packages from `.github/workflows/ci.yml`) before `cmake --preset debug`.

## 2. Configure

```sh
cmake --preset debug
```

This creates `build/debug/` and downloads dependencies on first run (can take a minute).

## 3. Build the game

```sh
cmake --build --preset debug --target game
```

## 4. Run it

```sh
# Windows
./build/debug/bin/game.exe
# Linux / macOS
./build/debug/bin/game
```

A 1280×720 window titled "Business game" opens on the main menu: a dark panel, a green start button, and a red exit button (no labels yet). Hover brightens the button under the cursor. Click start or press Enter to begin Arkanoid (paddle, ball, 50 bricks, 3 lives). Click exit to leave. Left/Right move the paddle. Escape pauses (world freezes; overlay dims the view). Escape again resumes; Enter on the overlay returns to the menu. Clearing the bricks or losing the last life also returns to the menu. The window chrome close control always exits.

## Next steps

Everyday commands and tests: [../how-to/build-and-test.md](../how-to/build-and-test.md). Layout: [../reference/source-layout.md](../reference/source-layout.md). Loop, screens, and Arkanoid: [../reference/application-loop.md](../reference/application-loop.md), [../reference/screens-and-input.md](../reference/screens-and-input.md), [../reference/world-and-levels.md](../reference/world-and-levels.md).
