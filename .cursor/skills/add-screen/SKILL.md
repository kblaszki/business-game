---
name: add-screen
description: >-
  There are no screens in this tree yet. Use when introducing a scene/screen layer;
  follow mvp/ notes, not MenuScreen from an old branch.
---

# Add a screen (not in the tree yet)

This skeleton has only `src/main.cpp` (SFML window) and `Example` in `gameLib`. There is no `ScreenI`, `ScreenStack`, `MenuScreen`, or `GameScreen`.

When the user asks for screens:

- Read [mvp/](../../../mvp/README.md) for the intended engine notes.
- Do **not** invent `MenuScreen` / `GameplayScreen` / `ScreenStack` unless the contract names them.
- New `.cpp` still goes in `gameLib` via `src/CMakeLists.txt`.
- Keep unit tests windowless unless a display is required.
- Update [docs/reference/source-layout.md](../../../docs/reference/source-layout.md) and [docs/index.md](../../../docs/index.md) in the same change.

Until then, do not add a screen layer unprompted.
