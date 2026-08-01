---
title: Design decisions
diataxis: explanation
audience: [ai, human]
related_code:
  - src/window/WindowI.hpp
  - src/managers/EventManagers.hpp
  - src/controllers/ScreenController.cpp
  - src/screens/ScreenUpdaterI.hpp
  - src/resources/ResourceManager.hpp
  - src/controllers/GameController.cpp
related_docs:
  - ../reference/interfaces.md
  - ../reference/architecture.md
keywords: [design, rationale, interface segregation, dependency injection, ISP, scene stack, fixed timestep, ResourceManager]
last_reviewed: 2026-08-01
---

# Design decisions

Why the code looks the way it does. For "what exists", see the reference docs.

## Interfaces everywhere (`*I`)

Nearly every collaborator is behind a small `*I` interface. This keeps SFML at the edges (`WindowSFML`) and lets tests inject gmock doubles (`tests/mocks/`). Screens depend on `ScreenRendererI`, entities on `DrawerI` — not on `sf::RenderWindow`.

## Interface segregation

`WindowI` is not one fat interface; it is composed from role interfaces (`WindowOpenerI`, `WindowCloserI`, `EventCollectorI`, `ScreenRendererI`, and its parts `CleanerI`/`DrawerI`/`DisplayerI`). Each consumer asks for the narrowest capability it needs: the game loop uses `isOpen`, entities use only `draw`. This limits blast radius and makes mocks tiny.

## Manual dependency injection

`main.cpp` constructs everything and passes dependencies by reference or `unique_ptr`, then hands ownership to `GameController`. There is no DI framework or globals; lifetime is explicit and ordered.

## Typed manager registry

`EventManagers` maps `ManagerOf` to managers and hands back a statically-typed reference via `get<ManagerOf::X>()`. `LordOfEventManagers::emplace<T>()` installs one, guarded by the `IsBaseOfEventManager` concept and the static `MANAGER_TYPE` constant. This trades a little template machinery for compile-time-safe lookups and no manual casting at call sites.

## Deferred screen switching

`ScreenController` keeps a scene stack. `pushScreen` / `popScreen` / `replaceScreen` stash a pending transition applied at the start of the next `update(dt)`, so a screen can replace or overlay itself from inside a button callback without deleting itself mid-execution. Only the top screen updates; all screens draw bottom-up for overlays.

## Handler lifetime via `ManagedList`

Registering a handler returns a move-only RAII `UnRegisterer` that erases the handler in its destructor (`[[nodiscard]]` on register APIs). Entities and handlers (`Paddle`, `OnClickHandler`, `OnHoverHandler`) store these handles as members — discarding a registration is a compile-time warning and would otherwise leave a dangling `std::function` into freed state.

## Fixed timestep

`GameController` accumulates wall-clock frame time and calls `update(FIXED_DT)` in constant 1/60 s steps. Entity speeds are expressed in px/s (e.g. `Paddle::SPEED_PX_PER_SEC`), so gameplay does not depend on the window framerate limit.

## Resource ownership

`ResourceManager` owns fonts (and later other assets), keyed by path relative to `<exe-dir>/resources`. Screens/entities borrow `const sf::Font&`; missing assets throw during load instead of logging and continuing.

## Letterboxed design resolution

`GameWindowManager` keeps a 1280×720 view and letterboxes it inside the real window on `Resized`. `WindowSFML` maps mouse pixels to world coordinates so UI hit-tests stay aligned with the letterboxed view.

## No namespaces

The codebase deliberately uses no namespaces; types are globally scoped with descriptive names and the `*I` convention. Match this when adding code.
