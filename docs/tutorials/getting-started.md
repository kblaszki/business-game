---
title: Getting started
diataxis: tutorial
audience: [ai, human]
related_code:
  - CMakePresets.json
  - CMakeLists.txt
  - cmake/FetchSFML.cmake
  - cmake/FetchGTest.cmake
  - src/main.cpp
  - src/Game.hpp
  - src/window/WindowSFML.hpp
  - src/time/ClockSFML.hpp
  - src/screen/ScreenI.hpp
  - src/screen/ScreenStack.hpp
  - src/screen/MainMenuScreen.hpp
  - src/screen/GameplayScreen.hpp
  - src/input/Action.hpp
  - src/input/InputMapper.hpp
  - src/screen/PauseScreen.hpp
  - src/world/World.hpp
  - .github/workflows/ci.yml
related_docs:
  - ../how-to/build-and-test.md
  - ../reference/source-layout.md
  - ../reference/input-and-events.md
  - ../reference/pause-overlay.md
  - ../../mvp/10-engine-progress.md
keywords: [setup, toolchain, build, run, msys2, ninja, cmake, first time]
last_reviewed: 2026-09-22
---

# Getting started

Goal: from a fresh clone to a running window. By the end you will have configured, built, and launched `game`.

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
cmake --build --preset debug --target game
```

## 4. Run it

```sh
# Windows
./build/debug/bin/game.exe
# Linux / macOS
./build/debug/bin/game
```

A 1280×720 window titled "Business game" opens on a menu bar. Enter starts a wrapping dummy. Escape (or losing focus) pauses — the dummy freezes; Escape resumes, Enter on the overlay returns to the menu. Close the window with the chrome.

## Next steps

Everyday commands and tests: [../how-to/build-and-test.md](../how-to/build-and-test.md). Layout: [../reference/source-layout.md](../reference/source-layout.md). Input: [../reference/input-and-events.md](../reference/input-and-events.md). Pause: [../reference/pause-overlay.md](../reference/pause-overlay.md). World: [../reference/world-and-levels.md](../reference/world-and-levels.md).
