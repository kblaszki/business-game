---
name: add-entity
description: >-
  Adds a new EntityI type under src/entities, wires it into gameLib and a screen.
  Use when creating game objects, paddles, balls, buttons, or any draw/update entity.
---

# Add entity

## Checklist

```
- [ ] Create src/entities/Name.hpp and Name.cpp
- [ ] Implement EntityI (update + draw)
- [ ] Add Name.cpp to gameLib in src/CMakeLists.txt
- [ ] Spawn in MenuScreen or GameScreen
- [ ] Register input via managers if needed
```

## Steps

1. **Header** — `#pragma once`, inherit `EntityI`, follow `Paddle` / `Button`:

```cpp
#include "EntityI.hpp"
#include <window/DrawerI.hpp>

class Name : public EntityI
{
public:
    void update() override;
    void draw(DrawerI& drawer) const override;
};
```

2. **Implementation** — draw through `DrawerI`, not raw window APIs from screens.

3. **CMake** — append `entities/Name.cpp` to the `gameLib` list in `src/CMakeLists.txt`. Without this, the file never builds.

4. **Screen** — `entities.emplace_back(std::make_unique<Name>(...));` in `GameScreen` or `MenuScreen` ctor (see `GameScreen.cpp`).

5. **Input** — inject `KeyboardManagerI` / `MouseManagerI` (or `EventManagers&`) like `Paddle` / `Button`; register handlers in the ctor and unregister on destroy if the existing pattern does so.

## References

- Interface: `src/entities/EntityI.hpp`
- Examples: `src/entities/Paddle.*`, `src/entities/Button.*`
- Do not treat `Ball.*` as a finished template until it is added to `gameLib` and matches project style (`#pragma once`).
