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
last_reviewed: 2026-09-22
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
| `src/entities/` | `EntityI` implementations: `Paddle`, `Ball`, `Brick`, `Button` |
| `src/handlers/` | Reusable UI behavior: `OnClickHandler`, `OnHoverHandler` |
| `src/window/` | Window interfaces (ISP) and `WindowSFML` implementation |
| `src/utils/` | `ManagedList`, `RectCollision` (AABB helpers) |
| `tests/unit_tests/` | GoogleTest suites (`controllers`, `handlers`, `managers`, `resources`, `entities`, `utils`, `screens`) |
| `tests/mocks/` | gmock doubles |
| `resources/fonts/` | Fonts copied next to the binary at build |

## Build targets

- **`gameLib`** (STATIC) — all implementation `.cpp` under `src/`, listed in `src/CMakeLists.txt`. Links SFML 3.1 Graphics/Window/System (`SFML::Graphics`, `SFML::System`, `SFML::Window`); audio and network are OFF in `cmake/FetchSFML.cmake`. C++23 (`cxx_std_23`).
- **`game`** (executable) — only `src/main.cpp`, links `gameLib`, C++23.
- Unit tests (Debug only) — GoogleTest 1.18 via `cmake/FetchGTest.cmake`. Linux GNU links HarfBuzz with `--whole-archive`; Windows GNU uses `LINK_GROUP(RESCAN)` for HarfBuzz+FreeType (whole-archive can deadlock at process start).

## Adding a source file

Any new `.cpp` under `src/` must be added to the `gameLib` source list in `src/CMakeLists.txt`, otherwise it is never compiled. Header-only files (`*.hpp`, interfaces) do not need listing.
