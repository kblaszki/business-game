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
| `src/main.cpp` | Entry point; composes window, resources, managers, screens, game loop |
| `src/controllers/` | `GameController` (loop), `EventController` (poll + route), `ScreenController` (scene stack) |
| `src/managers/` | Event managers keyed by `ManagerOf`: keyboard, mouse, game-exit, game-window, plus `EventManagers` container |
| `src/resources/` | `ResourceManager` (font cache, exe-relative paths) |
| `src/screens/` | `MenuScreen`, `GameScreen` (both `ScreenI`) |
| `src/entities/` | `EntityI` implementations: `Paddle`, `Button` |
| `src/handlers/` | Reusable UI behavior: `OnClickHandler`, `OnHoverHandler` |
| `src/window/` | Window interfaces (ISP) and `WindowSFML` implementation |
| `src/utils/` | `ManagedList` |
| `tests/unit_tests/` | GoogleTest suites (`controllers`, `handlers`, `managers`, `resources`, `entities`, `screens`) |
| `tests/mocks/` | gmock doubles |
| `resources/fonts/` | Fonts copied next to the binary at build |

## Build targets

- **`gameLib`** (STATIC) — all implementation `.cpp` under `src/`, listed in `src/CMakeLists.txt`. Links SFML 3 (`SFML::Graphics`, `SFML::System`, `SFML::Window`).
- **`game`** (executable) — only `src/main.cpp`, links `gameLib`.

## Adding a source file

Any new `.cpp` under `src/` must be added to the `gameLib` source list in `src/CMakeLists.txt`, otherwise it is never compiled. Header-only files (`*.hpp`, interfaces) do not need listing.
