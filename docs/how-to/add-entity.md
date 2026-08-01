---
title: Add a new entity
diataxis: how-to
audience: [ai, human]
related_code:
  - src/entities/EntityI.hpp
  - src/entities/Paddle.hpp
  - src/entities/Paddle.cpp
  - src/entities/Button.hpp
  - src/resources/ResourceManager.hpp
  - src/CMakeLists.txt
  - src/screens/GameScreen.cpp
related_docs:
  - ../reference/interfaces.md
  - ../reference/source-layout.md
keywords: [entity, EntityI, paddle, draw, update, spawn, ResourceManager]
last_reviewed: 2026-08-01
---

# Add a new entity

Goal: a new `EntityI` that a screen updates and draws.

```
- [ ] Create src/entities/Name.hpp and Name.cpp
- [ ] Implement EntityI (update + draw)
- [ ] Add entities/Name.cpp to gameLib in src/CMakeLists.txt
- [ ] Spawn it in a screen
- [ ] Register input via managers if needed
- [ ] Build and (if managers/controllers touched) add tests
```

## 1. Header

```cpp
#pragma once

#include "EntityI.hpp"
#include <window/DrawerI.hpp>

class Name : public EntityI
{
public:
    void update(float dt) override;
    void draw(DrawerI& drawer) const override;
};
```

`dt` is seconds (fixed timestep from `GameController`). Follow `src/entities/Paddle.hpp` for an input-driven object or `Button.hpp` for a UI element.

## 2. Implementation

Draw through the `DrawerI&` argument (e.g. `drawer.draw(shape);`). Do not reach for the raw SFML window from an entity.

## 3. Register in the build

Append `entities/Name.cpp` to the `gameLib` source list in `src/CMakeLists.txt`. Without this the file never compiles.

## 4. Spawn in a screen

In `GameScreen` (or `MenuScreen`) constructor:

```cpp
entities.emplace_back(std::make_unique<Name>(/* deps */));
```

See `src/screens/GameScreen.cpp` — the screen's `update(dt)`/`display()` loop already calls each entity.

## 5. Input (optional)

Inject the manager interface you need (`KeyboardManagerI&`, `MouseManagerI&`, or the whole `EventManagers&`) like `Paddle` / `Button`. Register handlers in the constructor and **store** the returned `UnRegisterer` as a member (RAII; `[[nodiscard]]`). See [interfaces.md](../reference/interfaces.md).

## 6. Fonts / assets (optional)

Borrow from `ResourceManager` (`resources.getFont("fonts/prototype.ttf")`) — do not load files from the CWD inside the entity. The manager is owned in `main.cpp` and passed into screens.

## Verify

`cmake --build --preset debug --target game`, then run and confirm the entity appears. Prefer a unit test under `tests/unit_tests/entities/` (see `PaddleTest.cpp` / `ButtonTest.cpp`), then `ctest --preset debug`.
