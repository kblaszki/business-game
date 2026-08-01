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
- [ ] Switch with screenUpdater.setScreen(...)
- [ ] Pass EventManagers, ScreenRendererI, ScreenUpdaterI like existing screens
```

## Steps

1. **Interface** — implement `ScreenI` (`update(float dt)`, `display`). Match ctor shape of existing screens:

```cpp
NameScreen(EventManagers& eventManagers,
           ScreenRendererI& screenRenderer,
           ScreenUpdaterI& screenUpdater);
```

2. **Lifecycle** — in `display()`, clear → draw entities via `screenRenderer` → `display()`, same as `MenuScreen` / `GameScreen`.

3. **CMake** — add `screens/NameScreen.cpp` to `gameLib` in `src/CMakeLists.txt`.

4. **Navigate** — call `ScreenUpdaterI::setScreen(std::make_unique<NameScreen>(...))` from a button callback or entity (see Start button in `MenuScreen.cpp`).

5. **Default entry** — `ScreenController` starts on `MenuScreen`; change that only if intentionally replacing the boot screen.

## References

- `src/screens/ScreenI.hpp`, `ScreenUpdaterI.hpp`
- `src/screens/MenuScreen.*`, `GameScreen.*`
- Controllers: `src/controllers/ScreenController.*`

## Update docs

After the change, refresh docs whose `related_code` lists the files you touched (at least `docs/how-to/add-screen.md`), and `docs/reference/source-layout.md` for the new `.cpp`. Use the `update-docs` skill for a full sweep.
