---
name: add-screen
description: >-
  Adds a new ScreenI, switches via ScreenUpdaterI, and follows MenuScreen/GameScreen
  patterns. Use when creating menus, game stages, pause screens, or screen transitions.
---

# Add screen

Source of truth: [docs/how-to/add-screen.md](../../../docs/how-to/add-screen.md) (details) and [docs/reference/architecture.md](../../../docs/reference/architecture.md). This skill is the quick path; keep it and the doc consistent.

## Checklist

```
- [ ] Create src/screens/NameScreen.hpp and NameScreen.cpp
- [ ] Implement ScreenI (update(float dt) + display)
- [ ] Add NameScreen.cpp to gameLib in src/CMakeLists.txt
- [ ] Switch with screenUpdater.replaceScreen / pushScreen
- [ ] Pass EventManagers, ScreenRendererI, ScreenUpdaterI, ResourceManager like existing screens
```

## Steps

1. **Interface** — implement `ScreenI` (`update(float dt)`, `display`). Match ctor shape of existing screens:

```cpp
NameScreen(EventManagers& eventManagers,
           ScreenRendererI& screenRenderer,
           ScreenUpdaterI& screenUpdater,
           ResourceManager& resources);
```

2. **Lifecycle** — in `display()`, only draw entities via `screenRenderer` (no clear/present; the controller owns that for the scene stack).

3. **CMake** — add `screens/NameScreen.cpp` to `gameLib` in `src/CMakeLists.txt`.

4. **Navigate** — `replaceScreen` for full transitions, `pushScreen`/`popScreen` for overlays (see Start button in `MenuScreen.cpp`).

5. **Default entry** — `main.cpp` injects the initial screen factory into `ScreenController`.

## References

- `src/screens/ScreenI.hpp`, `ScreenUpdaterI.hpp`
- `src/screens/MenuScreen.*`, `GameScreen.*`
- Controllers: `src/controllers/ScreenController.*`

## Update docs

After the change, refresh docs whose `related_code` lists the files you touched (at least `docs/how-to/add-screen.md`), and `docs/reference/source-layout.md` for the new `.cpp`. Use the `update-docs` skill for a full sweep.
