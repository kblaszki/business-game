---
title: Getting started
diataxis: tutorial
audience: [ai, human]
related_code:
  - CMakePresets.json
  - CMakeLists.txt
  - cmake/FetchSFML.cmake
  - src/Game.cpp
related_docs:
  - ../how-to/build-and-test.md
  - ../reference/source-layout.md
keywords: [setup, toolchain, build, run, msys2, ninja, cmake, first time]
last_reviewed: 2026-09-19
---

# Getting started

Goal: from a fresh clone to a running empty SFML window. By the end you will have configured, built, and launched `game`.

## 1. Install the toolchain

You need CMake, Ninja, and a C++20 compiler on your `PATH`. On Windows this repo is set up around MSYS2 MinGW.

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

SFML 3.1 and GoogleTest are fetched automatically by CMake (`cmake/FetchSFML.cmake`, `cmake/FetchGTest.cmake`). No separate install.

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

A 1280x720 window titled "Business game" opens. Close it with the window chrome close control.

## Next steps

Everyday commands and tests: [../how-to/build-and-test.md](../how-to/build-and-test.md). Layout of the scaffold: [../reference/source-layout.md](../reference/source-layout.md).
