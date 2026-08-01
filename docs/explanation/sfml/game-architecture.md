---
title: Game architecture on SFML 3
diataxis: explanation
audience: [ai, human]
related_code:
  - src/main.cpp
  - src/controllers/GameController.cpp
  - src/controllers/ScreenController.cpp
  - src/resources/ResourceManager.hpp
  - src/managers/GameWindowManager.cpp
  - src/window/WindowSFML.cpp
related_docs:
  - ./index.md
  - ./events.md
  - ./screens-pause-levels.md
  - ../../reference/architecture.md
  - ../design-decisions.md
keywords: [SFML, game loop, fixed timestep, architecture, DI, resources, view, letterbox, EntityI]
last_reviewed: 2026-08-01
---

# Game architecture on SFML 3

SFML gives you a window, a clock, drawable primitives, and an event queue. Structure beyond that is yours. This page summarizes patterns that work well for mid-sized C++2D games and how business-game applies them.

## The classic loop

Almost every SFML game reduces to:

```text
while (window.isOpen())
{
    handleEvents();
    update(dt);
    clear();
    draw();
    display();
}
```

SFML’s `setFramerateLimit` / vsync only caps **rendering**. If movement is “N pixels per frame” with no clock, gameplay speed changes with the machine. Prefer measuring real time with `sf::Clock` (or an injectable clock in tests).

## Fixed timestep vs variable dt

| Approach | Idea | Pros | Cons |
|----------|------|------|------|
| **Variable dt** | `position += velocity * frameSeconds` | Simple | Physics/collisions diverge under hitch; hard to replay |
| **Fixed timestep** | Accumulate wall time; run `update(FIXED_DT)` zero or more times | Stable simulation; easier tests | Need spiral-of-death clamp; optional render interpolation |

For anything with collisions (Arkanoid-style balls, platformers), **fixed timestep** is the usual recommendation. A common constant is `1/60` seconds. Clamp a single frame’s contribution (e.g. max 0.25 s) so a long stall does not run hundreds of catch-up steps.

Optional refinement: interpolate render state between the last two simulation states for smoothness when the display rate ≠ sim rate. Many prototypes skip interpolation until hitching is visible.

## Layering: keep SFML at the edges

A maintainable split:

```text
main / composition root
    → game loop controller
        → event facade (poll + dispatch)
        → screen / scene stack
            → entities / gameplay types
    → window adapter (SFML RenderWindow behind interfaces)
    → resource cache (fonts, textures, later sounds)
```

**Why:** unit tests mock `DrawerI` / `ScreenRendererI` / `EventCollectorI` without opening a window; swapping SFML versions or backends touches the adapter, not every paddle.

**Anti-patterns to avoid early**

- Global `sf::RenderWindow*` reachable from every entity
- Loading the same font from disk in every `Button` constructor
- One mega-`Game` class that owns window, levels, networking, and UI

## Resources

Own assets in one place (path → `sf::Font` / `sf::Texture` / later `sf::SoundBuffer`). Borrow references into screens and entities. Resolve the resources root relative to the **executable**, not the process CWD — players launch from shortcuts and IDEs with different working directories.

Fail loudly on missing critical assets (throw at load) rather than logging and drawing blank text.

## Views, resolution, and letterboxing

`sf::View` maps world coordinates to the window. Two common strategies:

1. **Stretch** the design resolution to the window (simple; distorts aspect).
2. **Letterbox / pillarbox** a fixed design size (e.g. 1280×720) inside the real framebuffer (black bars; correct aspect).

If you letterbox, convert mouse **pixel** positions into **world** coordinates with the same view transform; otherwise UI hit-tests drift when the window is resized.

`Resized` events are the hook to recompute the viewport.

## Screens vs entities

| Layer | Responsibility |
|-------|----------------|
| **Screen** | Mode of the program: menu, playing, pause overlay; owns high-level state; decides transitions |
| **Entity** | Drawable/updatable object with local behavior (paddle, brick, button) |

Two ways to structure gameplay objects inside a screen:

1. **Typed members** — `Paddle`, `Ball`, `std::vector<Brick>` on `GameScreen`. Collisions and win/lose stay readable in one `update`.
2. **Homogeneous list** — `vector<unique_ptr<EntityI>>` plus systems that iterate everyone. Flexible; costs indirection and makes “ball vs brick” logic harder without casting or extra registries.

For a single Arkanoid-like session, typed members are enough. Introduce a generic entity bus only when many screens share the same object soup.

## Controllers as orchestration

A useful split of “who runs the loop”:

- **Event controller** — drain SFML queue, route to input/window managers
- **Screen controller** — scene stack, deferred transitions, clear/draw/display
- **Game controller** — while open: clock → events → fixed updates → render

Screens should not call `window.clear()` / `window.display()` if the screen controller owns the frame; otherwise overlays and multi-screen draw order break.

## Audio and network (when you need them)

Add modules when a feature requires them:

- **Audio:** short SFX via `sf::Sound` + cached `sf::SoundBuffer`; long tracks via `sf::Music` streaming from disk. Respect `FocusLost` (pause/mute).
- **Network:** SFML 3.1’s TLS/HTTPS/IPv6 help for APIs and downloads; game lockstep/realtime still needs your own protocol design on top of sockets.

Do not add empty “AudioManager” facades “for later.”

## In business-game

Concrete wiring and the fixed-step loop are documented in [architecture.md](../../reference/architecture.md). Rationale for interfaces, DI, and the scene stack: [design-decisions.md](../design-decisions.md).

Summary of choices already in code:

| Concern | Choice |
|---------|--------|
| Loop | `GameController`: accumulator, `FIXED_DT = 1/60`, max frame clamp 0.25 s |
| Events | `EventController` + typed managers ([events.md](./events.md)) |
| SFML edge | `WindowSFML` implements narrow `*I` ports |
| Composition | Manual DI in `main.cpp` |
| Resources | `ResourceManager` under `<exe>/resources` |
| View | `GameWindowManager` letterboxes design 1280×720; mouse mapped to world |
| Gameplay | Typed `Paddle` / `Ball` / `Brick` on `GameScreen` |

Next: [Screens, pause, and levels](./screens-pause-levels.md).
