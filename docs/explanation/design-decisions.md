---
title: Design decisions
diataxis: explanation
audience: [ai, human]
related_code:
  - src/window/WindowI.hpp
  - src/managers/EventManagers.hpp
  - src/controllers/ScreenController.cpp
  - src/screens/ScreenUpdaterI.hpp
related_docs:
  - ../reference/interfaces.md
  - ../reference/architecture.md
keywords: [design, rationale, interface segregation, dependency injection, ISP, deferred swap]
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

`ScreenController::setScreen` stores the next screen in `newScreen` and swaps it in at the start of the next `update()`. A screen can therefore request its own replacement from inside a button callback without deleting itself mid-execution.

## Handler lifetime via `ManagedList`

Registering a handler returns an `UnRegisterer`; when it is destroyed the handler is removed. Objects like `OnClickHandler` hold their unregisterer, so their callbacks disappear automatically when the object dies — no dangling `std::function` into freed state.

## No namespaces

The codebase deliberately uses no namespaces; types are globally scoped with descriptive names and the `*I` convention. Match this when adding code.
