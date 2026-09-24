---
title: Getting started
diataxis: tutorial
audience: [ai, human]
related_code:
  - CMakePresets.json
  - CMakeLists.txt
  - cmake/FetchSFML.cmake
  - cmake/FetchGTest.cmake
  - games/arkanoid/main.cpp
  - games/arkanoid/app/include/arkanoid/app/Scenes.hpp
  - games/arkanoid/sim/include/arkanoid/sim/State.hpp
  - .github/workflows/ci.yml
related_docs:
  - ../how-to/build-and-test.md
  - ../reference/source-layout.md
  - ../reference/input-and-events.md
  - ../reference/pause-overlay.md
  - ../reference/arkanoid-sim.md
  - ../explanation/architecture.md
  - ../../mvp/10-engine-progress.md
keywords: [setup, toolchain, build, run, msys2, ninja, cmake, first time, arkanoid]
last_reviewed: 2026-09-24
---

# Getting started

Goal: from a fresh clone to a running window. By the end you will have configured, built, and launched `arkanoid`.

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

SFML 3.1 (Graphics/Window/System only) and GoogleTest 1.18 are fetched automatically by CMake (`cmake/FetchSFML.cmake`, `cmake/FetchGTest.cmake` in Debug). No separate SFML install.

## 2. Configure

```sh
cmake --preset debug
```

This creates `build/debug/` and downloads dependencies on first run (can take a minute).

## 3. Build the game

```sh
cmake --build --preset debug --target arkanoid
```

## 4. Run it

```sh
# Windows
./build/debug/bin/arkanoid.exe
# Linux / macOS
./build/debug/bin/arkanoid
```

A 1280×720 window opens on a Start / Quit menu. Start or Enter plays Stage1 (arrows or A/D move the paddle, Enter launches). Clearing a stage loads the next; Stage3 is the last. Escape pauses. Backspace on the menu quits. Donor bricks drop Wide, MultiBall, Slow, and ExtraLife capsules.

## Next steps

Everyday commands and tests: [../how-to/build-and-test.md](../how-to/build-and-test.md). Layout: [../reference/source-layout.md](../reference/source-layout.md). Input: [../reference/input-and-events.md](../reference/input-and-events.md). Pause: [../reference/pause-overlay.md](../reference/pause-overlay.md). Sim rules: [../reference/arkanoid-sim.md](../reference/arkanoid-sim.md).
