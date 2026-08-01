---
title: Source layout reference
diataxis: reference
audience: [ai, human]
related_code:
  - src/CMakeLists.txt
  - CMakeLists.txt
  - tests/unit_tests/CMakeLists.txt
related_docs:
  - ./architecture.md
  - ../how-to/build-and-test.md
keywords: [layout, directories, gameLib, game, targets, cmake sources]
last_reviewed: 2026-08-01
---

# Source layout reference

## Directories

| Path | Responsibility |
|------|----------------|
| `src/main.cpp` | Entry point; composes window, managers, screens, game loop |
| `src/controllers/` | `GameController` (loop), `EventController` (poll + route), `ScreenController` (screen swap) |
| `src/managers/` | Event managers keyed by `ManagerOf`: keyboard, mouse, game-exit, plus `EventManagers` container |
| `src/screens/` | `MenuScreen`, `GameScreen` (both `ScreenI`) |
| `src/entities/` | `EntityI` implementations: `Paddle`, `Button`, `Player` |
| `src/handlers/` | Reusable UI behavior: `OnClickHandler`, `OnHoverHandler` |
| `src/window/` | Window interfaces (ISP) and `WindowSFML` implementation |
| `src/utils/` | `ManagedList`, `EventPrinter` |
| `tests/unit_tests/` | GoogleTest suites, mirror `src/` layout (Debug only) |
| `tests/mocks/` | gmock doubles |
| `resources/fonts/` | Fonts copied next to the binary at build |

## Build targets

- **`gameLib`** (SHARED) — all implementation `.cpp` under `src/`, listed in `src/CMakeLists.txt`. Links SFML (`sfml-graphics`, `sfml-system`, `sfml-window`).
- **`game`** (executable) — only `src/main.cpp`, links `gameLib`.

## Adding a source file

Any new `.cpp` under `src/` must be added to the `gameLib` source list in `src/CMakeLists.txt`, otherwise it is never compiled. Header-only files (`*.hpp`, interfaces) do not need listing.

## Not in the build

- `src/entities/Ball.cpp` / `Ball.hpp` are WIP stubs, not listed in `gameLib`.
- `src/entities/Player.*` compiles into `gameLib` but is not used by any screen yet.
- `src/managers/EntitiesManagerI.hpp` is an interface sketch with no implementation.
