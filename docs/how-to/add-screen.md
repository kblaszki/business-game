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
keywords: [screen, ScreenI, ScreenUpdaterI, setScreen, transition, menu]
last_reviewed: 2026-08-01
---

# Add a new screen

Goal: a new `ScreenI` reachable via a transition.

```
- [ ] Create src/screens/NameScreen.hpp and NameScreen.cpp
- [ ] Implement ScreenI (update + display)
- [ ] Add screens/NameScreen.cpp to gameLib in src/CMakeLists.txt
- [ ] Trigger it with screenUpdater.setScreen(...)
- [ ] Build and run
```

## 1. Match the existing screen shape

Constructors of `MenuScreen` / `GameScreen` take the same three dependencies:

```cpp
NameScreen(EventManagers& eventManagers,
           ScreenRendererI& screenRenderer,
           ScreenUpdaterI& screenUpdater);
```

Implement `ScreenI`: `update()` iterates entities; `display()` does clear -> draw entities -> display through `screenRenderer` (copy the pattern in `src/screens/GameScreen.cpp`).

## 2. Register in the build

Append `screens/NameScreen.cpp` to the `gameLib` source list in `src/CMakeLists.txt`.

## 3. Navigate to it

From a button callback or entity, request the swap:

```cpp
screenUpdater.setScreen(
    std::make_unique<NameScreen>(eventManagers, screenRenderer, screenUpdater));
```

`ScreenController` applies the swap on the next `update()` (deferred, see [architecture.md](../reference/architecture.md)). The Start button in `src/screens/MenuScreen.cpp` is the reference example.

## 4. Boot screen

`ScreenController` starts on `MenuScreen` (`src/controllers/ScreenController.cpp`). Only change that constructor if you intend to replace the initial screen.

## Verify

`cmake --build --preset debug --target game`, run, and confirm the transition works.
