---
title: Game development bibliography
diataxis: explanation
audience: [ai, human]
related_code: []
related_docs:
  - ./index.md
  - ./from-scratch.md
  - ./multithreading.md
  - ../sfml/index.md
keywords: [bibliography, sources, SFML, game loop, references, best practices]
last_reviewed: 2026-08-01
---

# Bibliography

Primary external sources behind [from-scratch.md](./from-scratch.md) and [multithreading.md](./multithreading.md). Prefer these over blog posts of unknown quality when deciding architecture.

## Role legend

| Role | Meaning |
|------|---------|
| **Primary** | Read first; shapes day-to-day SFML practice |
| **Deep dive** | Stronger theory or engine context |
| **Reference** | Look up when implementing a specific feature |

## SFML official

| Source | Role | What you learn |
|--------|------|----------------|
| [SFML tutorials (3.1)](https://www.sfml-dev.org/tutorials/3.1/) | Primary | Window, events, graphics, audio, network — the supported API surface |
| [Events explained](https://www.sfml-dev.org/tutorials/3.1/window/events/) | Primary | `pollEvent` → `optional`, `is`/`getIf`, `handleEvents` visitation |
| [Using OpenGL in an SFML window](https://www.sfml-dev.org/tutorials/3.1/window/opengl/) | Reference | Context activation; **rendering from another thread** (`setActive`); window on main thread |
| [Custom audio streams](https://www.sfml-dev.org/tutorials/3.1/audio/streams/) | Reference | Stream playback on an SFML thread; mutex shared data |
| [FAQ — General](https://www.sfml-dev.org/faq/general/) | Primary | Platforms, modules, “no 3D engine,” language bindings |
| [SFML 3.1.0 release](https://github.com/SFML/SFML/releases/tag/3.1.0) | Reference | Text shaping, TLS/HTTPS, mobile fixes, QOI, `sf::version()` |
| [Migrate SFML 2 → 3](https://www.sfml-dev.org/tutorials/3.1/migration/sfml-2/) | Primary | C++17, vector2 APIs, event redesign, CMake `SFML::` targets |
| [Migrate SFML 3.0 → 3.1](https://www.sfml-dev.org/tutorials/3.1/migration/sfml-3.0/) | Reference | HarfBuzz text, network TLS, deprecations |
| [API docs 3.1](https://www.sfml-dev.org/documentation/3.1.0/) | Reference | Exact class/function contracts |

## SFML project structure (books & samples)

| Source | Role | What you learn |
|--------|------|----------------|
| Artur Moreira, Henrik Vogelius Hansson, Jan Haller — *SFML Game Development* (Packt) | Primary | `Game` class, fixed timestep loop, resource holders, **state stack**, world/player separation |
| [SFML-Game-Development-Book (code)](https://github.com/SFML/SFML-Game-Development-Book) | Primary | Runnable reference for the book’s chapter progression (note: written for SFML 2 — port patterns, not copy event APIs blindly) |
| [Game Code School — Simple game engine (SFML 3)](https://gamecodeschool.com/sfml/simple-game-engine.html) | Primary | Thin `main`, `Engine` owns loop, split input/update/draw files, entity class alongside engine |
| [SFML forum — How should the code be organized?](https://en.sfml-dev.org/forums/index.php?topic=17069.0) | Deep dive | Community patterns: engine + scenes, resources vs sprites, observer-style game events |

## Game loop and time (language-agnostic)

| Source | Role | What you learn |
|--------|------|----------------|
| [Glenn Fiedler — Fix Your Timestep!](https://gafferongames.com/post/fix_your_timestep/) | Primary | Accumulator, fixed `dt`, spiral-of-death clamp, render interpolation |
| [Robert Nystrom — Game Programming Patterns: Game Loop](http://gameprogrammingpatterns.com/game-loop.html) | Primary | Fixed update / variable render; tradeoffs of sync strategies |
| [Game Programming Patterns: Update Method](http://gameprogrammingpatterns.com/update-method.html) | Deep dive | Per-entity `update` in a frame |
| [Game Programming Patterns: State](http://gameprogrammingpatterns.com/state.html) | Deep dive | Mode/behavior state machines (menus, entity AI) |

## Broader engine context

| Source | Role | What you learn |
|--------|------|----------------|
| Jason Gregory — *Game Engine Architecture* | Deep dive | How large engines structure loops, subsystems, and assets — use selectively; do not import AAA complexity into a small SFML game |
| [Mike Acton — Data-Oriented Design (CppCon)](https://www.youtube.com/watch?v=rX0ItVEVjHc) | Deep dive | When data layout and batching matter; relevant only after profiling shows CPU limits |

## Multithreading-specific notes

| Source | Role | What you learn |
|--------|------|----------------|
| SFML OpenGL tutorial (above) | Primary | Official multi-thread render sketch and context rules |
| SFML audio streams tutorial (above) | Primary | Audio thread + mutex requirements |
| [Stack Overflow — SFML window on a separate thread](https://stackoverflow.com/questions/62409791/how-do-you-control-a-sfml-window-from-a-separate-thread) | Reference | Why blocking main for loads while moving the window to a side thread fails UX/OS expectations |

## Suggested reading order

1. SFML 3.1 window + events tutorials  
2. Fix Your Timestep + GPP Game Loop  
3. Packt book chapters on game class, resources, and states (or the GitHub samples)  
4. OpenGL-in-window + audio streams sections on threading  
5. Gregory / Acton only if you outgrow simple loops and entity lists  

Back to the [index](./index.md).
