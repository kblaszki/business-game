# Architecture review

Assessment date: 2026-08-01. Overall verdict: **the foundations are good for an SFML game** — small role interfaces, dependency injection, testability, clean layering. The problems are correctness details and missing game-engine fundamentals (time step, resources, scene stack), not a wrong architecture. A rewrite is not warranted; a targeted refactor is.

## What is good — keep it

| Pattern | Where | Why it works |
|---------|-------|--------------|
| Interface segregation | `src/window/WindowI.hpp` composed of `WindowOpenerI`, `WindowCloserI`, `EventCollectorI`, `ScreenRendererI` (itself `CleanerI`+`DrawerI`+`DisplayerI`) | Consumers get the narrowest capability; mocks stay tiny; SFML is quarantined in `WindowSFML` |
| Manual DI from `main.cpp` | `src/main.cpp` | Explicit construction order and ownership, no globals/singletons |
| Typed manager registry | `EventManagers::get<ManagerOf::X>()` + `LordOfEventManagers::emplace<T>()` with C++20 concepts | Compile-time-safe lookup, no casting at call sites |
| Deferred screen swap | `ScreenController::setScreen` stores `newScreen`, applies on next `update()` | A screen can replace itself from its own button callback without self-deleting mid-call |
| Test infrastructure | `tests/mocks/` + `tests/unit_tests/` mirroring `src/`, `StrictMock` | Controllers/managers/handlers are genuinely unit-testable |

These patterns are exactly what you want when building on SFML: keep the library at the edges and the game logic testable. Do not trade them away during the refactor.

## Issues, ordered by severity

### 1. Dangling handler bug: `Paddle` never unregisters (correctness, HIGH)

`ManagedList::emplace` returns an `UnRegisterer` (`std::function<void()>`) that must be **called** to remove the handler. `Paddle`'s constructor (`src/entities/Paddle.cpp`) discards both return values of `registerKeyHandler`. When the screen owning the `Paddle` is destroyed, the lambdas capturing `this` remain inside `KeyboardManager` — the next Left/Right key press is undefined behavior. `main.cpp` discards the Escape unregisterer too (harmless only because it must live forever). `OnClickHandler`/`OnHoverHandler` do it correctly but rely on remembering to call it in the destructor.

The API invites this bug: unregistration is opt-in instead of automatic.

```
- [x] Make unregistration RAII: replace ManagedList<T>::UnRegisterer with a move-only handle that calls the eraser in its destructor (or wrap in a small ScopedRegistration type)
- [x] Mark the register methods [[nodiscard]] on every manager interface
- [x] Store the handles as members in Paddle (and any future entity registering handlers)
- [x] Keep OnClickHandler/OnHoverHandler storing the handle; delete their now-redundant manual destructor calls
- [x] Add a unit test: destroy an entity, fire the event, assert no handler runs
```

### 2. No delta time / fixed timestep (gameplay, HIGH)

`GameController::run()` does `handleEvents / update / display` with no clock. `Paddle::update()` moves a fixed 10 px per frame, so game speed is dictated by `setFramerateLimit(60)` in `WindowSFML`. On a machine where vsync/limit behaves differently, gameplay speed changes.

```
- [x] Add sf::Clock to the game loop; compute dt each iteration
- [x] Change EntityI::update() and ScreenI::update() to update(float dt) (seconds)
- [x] Express Paddle speed in px/s (e.g. 600.f * dt)
- [x] Prefer a fixed timestep with accumulator (update in constant steps, render freely) so future physics/collisions stay deterministic
- [x] Keep the framerate limit only as a render cap, not as the source of game speed
```

### 3. Resource loading: each `Button` loads a font from disk (resources, MEDIUM)

`Button`'s constructor calls `font.loadFromFile("resources/fonts/prototype.ttf")` — one disk load and one `sf::Font` per button instance, hardcoded path, resolved relative to the process CWD (running the exe from another directory breaks rendering silently; errors go to `std::cerr` and are otherwise swallowed).

```
- [x] Add a small ResourceManager (font cache keyed by path; sf::Font owned once, handed out by reference)
- [x] Inject it from main.cpp into screens the same way EventManagers is injected
- [x] Resolve the resources root robustly (relative to the executable path, not CWD)
- [x] Fail loudly on missing resources (throw during construction) instead of logging and continuing
```

### 4. Per-event logging to `std::cerr` (noise/perf, MEDIUM)

`EventController::handleEvents()` prints every SFML event (including every mouse move) via `EventPrinter`. That is debug scaffolding running in release builds.

```
- [x] Remove the std::cerr line from EventController::handleEvents (done in phase 2)
- [x] EventPrinter deleted with it — it was pure SFML-2 API; re-add a debug-only logger later if tracing is ever wanted
```

### 5. Single current screen, no scene stack (architecture, MEDIUM)

`ScreenController` holds exactly one screen and hardcodes `MenuScreen` as the boot screen in its constructor. A pause menu, settings overlay, or dialog cannot be layered on top of a running game screen; the game screen would be destroyed and rebuilt.

```
- [x] Evolve ScreenController into a scene stack: push/pop/replace operations (extend ScreenUpdaterI accordingly)
- [x] update() the top scene only; display() all scenes bottom-up so overlays render above the game
- [x] Take the initial screen as a constructor argument (factory or unique_ptr) instead of hardcoding MenuScreen — also makes the controller testable without MenuScreen
```

### 6. Silently dropped event categories (completeness, LOW)

`getManagerOf` maps `Resized`/focus events to `ManagerOf::GameWindow` and joystick/touch/sensor to their enums, but no manager exists for any of them — the events vanish. Resizing the window today distorts rendering (no view adjustment).

```
- [x] Implement a GameWindowManager handling Resized (adjust sf::View / letterbox) and focus changes (auto-pause later)
- [x] Leave Joystick/Touch/Sensor unimplemented until needed; delete the enum values or document them as reserved
```

### 7. Dead and stub code (hygiene, LOW)

- `src/entities/Player.*` compiles into `gameLib` but no screen uses it; it overlaps with `Paddle`.
- `src/entities/Ball.cpp` is empty; `Ball.hpp` is a stub outside the build using include guards instead of `#pragma once`.
- `src/managers/EntitiesManagerI.hpp` sketches a collision/entity-management API with no implementation and no consumer.

```
- [x] Decide Player: delete it (Paddle covers the use case) or repurpose it as the first business-game domain entity
- [x] Decide Ball: either implement it properly (EntityI, gameLib entry, spawned by GameScreen, moves with dt, collides with Paddle) or delete both files
- [x] Keep EntitiesManagerI only if collisions are the next milestone; otherwise remove and reintroduce when needed
```

### 8. Test gaps (quality, LOW)

Controllers, managers, and handlers have suites; screens and entities have none — precisely the code the refactor will touch most.

```
- [ ] Add ScreenRendererI-based tests for MenuScreen/GameScreen (update/display call through to entities)
- [ ] Add Paddle tests (movement with dt, handler unregistration)
- [ ] Add Button tests via mouse manager mock (hover/click transitions)
```

## Is this approach right for an SFML game?

Yes. The common failure mode of SFML projects is raw `sf::RenderWindow` and event handling smeared across every class; this project already avoids that with the window abstraction and manager dispatch. What it is missing are the standard game-loop fundamentals — delta time, resource cache, scene stack — which bolt onto the existing design without changing its shape. The concrete order of work is in [refactoring-roadmap.md](refactoring-roadmap.md).
