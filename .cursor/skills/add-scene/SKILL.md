---
name: add-scene
description: >-
  Adds a SceneI implementation under games/arkanoid/app, wires it into
  arkanoid_app CMakeLists and SceneStack factories. Use when introducing or
  extending game scenes (menu, gameplay, pause, overlays).
---

# Add a scene

Source of truth: [docs/reference/engine-scene.md](../../../docs/reference/engine-scene.md), [docs/reference/arkanoid-app.md](../../../docs/reference/arkanoid-app.md), [docs/explanation/architecture.md](../../../docs/explanation/architecture.md).

Concrete game scenes live in `games/arkanoid/app/` (`namespace sgl::arkanoid`), not in `engine/scene/`. Engine provides `SceneI`, `SceneStack`, `SceneRequest`, `SceneTraits`.

## Checklist

```
- [ ] Create FooScene.hpp / FooScene.cpp under games/arkanoid/app/
- [ ] Implement sgl::SceneI (update, render, traits)
- [ ] Add FooScene.cpp to games/arkanoid/app/CMakeLists.txt
- [ ] Expose a SceneFactory from Scenes.hpp / Scenes.cpp if the stack needs it
- [ ] Cover with a windowless test under tests/arkanoid/app/
- [ ] Update docs/reference/arkanoid-app.md and source-layout.md if structure changed
```

## Steps

1. **Traits** — set `opaque`, `blocksUpdate`, `pausable` appropriately. Pause overlay: `opaque` false, `blocksUpdate` true, `pausable` false. Gameplay: `pausable` only while playing.

2. **Requests** — use `ctx.request(PushScene|PopScene|ReplaceScene|RequestPause|QuitApp)`. Never close the platform from a scene.

3. **Pause** — only `RequestPause` pushes the pause factory; do not push `PauseScene` ad hoc. See [pause-overlay.md](../../../docs/reference/pause-overlay.md).

4. **CMake** — list the new `.cpp` in `games/arkanoid/app/CMakeLists.txt` only.

5. **Tests** — extend `arkanoid_scenes_test` or add a suite; no window.

Do not invent `MenuScreen` / `ScreenStack` / `ScreenI` — those names are gone. Use `SceneI` / `SceneStack`.
