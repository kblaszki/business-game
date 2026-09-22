---
title: Add a new screen
diataxis: how-to
audience: [ai, human]
related_code:
  - src/screens/ScreenI.hpp
  - src/screens/ScreenUpdaterI.hpp
  - src/screens/MenuScreen.cpp
  - src/screens/GameScreen.cpp
  - src/controllers/ScreenController.cpp
  - src/CMakeLists.txt
related_docs:
  - ../reference/interfaces.md
  - ../reference/architecture.md
  - ../explanation/sfml/screens-pause-levels.md
keywords: [screen, ScreenI, ScreenUpdaterI, pushScreen, popScreen, replaceScreen, transition, menu]
last_reviewed: 2026-09-22
---

# Add a new screen

Goal: a new `ScreenI` reachable via a transition. The project is C++23 (`cxx_std_23` on `gameLib`). For when to push vs replace, pause overlays, and level changes, see [screens, pause, and levels](../explanation/sfml/screens-pause-levels.md).

```
- [ ] Create src/screens/NameScreen.hpp and NameScreen.cpp
- [ ] Implement ScreenI (update(float dt) + display)
- [ ] Add screens/NameScreen.cpp to gameLib in src/CMakeLists.txt
- [ ] Trigger it with screenUpdater.replaceScreen / pushScreen
- [ ] Build and run
```

## 1. Match the existing screen shape

Constructors of `MenuScreen` / `GameScreen` take:

```cpp
NameScreen(EventManagers& eventManagers,
           ScreenRendererI& screenRenderer,
           ScreenUpdaterI& screenUpdater,
           ResourceManager& resources);
```

Implement `ScreenI`: `update(float dt)` iterates entities (pass `dt` through); `display()` only draws content through `screenRenderer` — do **not** call `clear()` / `display()` on the renderer (the controller owns that for the scene stack). Copy `src/screens/GameScreen.cpp`.

## 2. Register in the build

Append `screens/NameScreen.cpp` to the `gameLib` source list in `src/CMakeLists.txt`.

## 3. Navigate to it

From a button callback or entity:

```cpp
// Replace the whole stack (e.g. Menu → Game)
screenUpdater.replaceScreen(
    std::make_unique<NameScreen>(eventManagers, screenRenderer, screenUpdater, resources));

// Or push an overlay (pause / dialog) and later popScreen()
screenUpdater.pushScreen(std::make_unique<PauseScreen>(...));
```

`ScreenController` applies the transition on the next `update(dt)` (deferred, see [architecture.md](../reference/architecture.md)). The Start button in `src/screens/MenuScreen.cpp` is the reference example for `replaceScreen`.

## 4. Boot screen

`main.cpp` passes a factory into `ScreenController` that builds `MenuScreen`. Change that factory to replace the initial screen.

## Verify

`cmake --build --preset debug --target game`, run, and confirm the transition works.
