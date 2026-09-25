---
name: add-entity
description: >-
  Adds a new type under engine/ or games/<name>/, wires it into that module's
  CMakeLists, and covers it with a windowless test. Use when creating a library
  type in sgl, sgl::arkanoid, or sgl::tetris.
---

# Add a type to an engine or game module

Source of truth: [docs/reference/source-layout.md](../../../docs/reference/source-layout.md) and [docs/explanation/architecture.md](../../../docs/explanation/architecture.md).

## Checklist

```
- [ ] Create Header.hpp / Name.cpp under the owning module
- [ ] Add Name.cpp to that module’s CMakeLists.txt
- [ ] Add or extend a test under tests/engine/<module>/ or tests/<name>/<sim|app>/
- [ ] Register the test with add_unit_test if it is a new suite
```

## Steps

1. **Header** — `#pragma once`, `namespace sgl` or `sgl::<name>`, one primary type, CamelCase:

```cpp
#pragma once

namespace sgl
{

class Name
{
public:
    int doWork() const;
};

} // namespace sgl
```

2. **Implementation** — keep sim and most app code SFML-free. Only `engine/sfml` and `games/<name>/main.cpp` may include SFML. A new title follows [add-a-game](../../../docs/how-to/add-a-game.md).

3. **CMake** — append `Name.cpp` to the owning module’s `CMakeLists.txt`. Without this, the file never builds.

4. **Test** — windowless GoogleTest via `add_unit_test` in the matching tests leaf. Suites stay headless except `event_translate_test` (SFML Window link, no window).

## References

- Layout: [docs/reference/source-layout.md](../../../docs/reference/source-layout.md)
- Contract: [docs/explanation/architecture.md](../../../docs/explanation/architecture.md)

## Update docs

After the change, refresh docs whose `related_code` lists the files you touched (at least `docs/reference/source-layout.md`). Use the `update-docs` skill for a full sweep.
