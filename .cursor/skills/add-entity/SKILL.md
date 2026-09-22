---
name: add-entity
description: >-
  Adds a new type under src/, wires it into gameLib, and covers it with a windowless test.
  Use when creating a library class next to Example, or any draw/update game object later.
---

# Add a type to gameLib

Source of truth: [docs/reference/source-layout.md](../../../docs/reference/source-layout.md). There is no `EntityI` in this tree yet. Follow `Example`.

## Checklist

```
- [ ] Create src/Name.hpp and Name.cpp
- [ ] Add Name.cpp to gameLib in src/CMakeLists.txt
- [ ] Add or extend a test under tests/unit_tests/
- [ ] Register the test with add_unit_test if it is a new suite
```

## Steps

1. **Header** — `#pragma once`, one primary type, follow `Example`:

```cpp
#pragma once

class Name
{
public:
    int doWork() const;
};
```

2. **Implementation** — keep `gameLib` windowless unless the type truly needs SFML. Put window code in `src/main.cpp` (or a later screen layer), not in the first library types.

3. **CMake** — append `Name.cpp` to the `gameLib` list in `src/CMakeLists.txt`. Without this, the file never builds.

4. **Test** — windowless GoogleTest via `add_unit_test` in `tests/unit_tests/CMakeLists.txt`. See `ExampleTest.cpp`.

## References

- Example: `src/Example.hpp`, `src/Example.cpp`
- Test: `tests/unit_tests/ExampleTest.cpp`

## Update docs

After the change, refresh docs whose `related_code` lists the files you touched (at least `docs/reference/source-layout.md`). Use the `update-docs` skill for a full sweep.
