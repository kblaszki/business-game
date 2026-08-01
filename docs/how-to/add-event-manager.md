---
title: Add a new event manager
diataxis: how-to
audience: [ai, human]
related_code:
  - src/managers/ManagerOf.hpp
  - src/managers/ManagerOf.cpp
  - src/managers/EventManagerI.hpp
  - src/managers/EventManagers.hpp
  - src/managers/KeyboardManagerI.hpp
  - src/managers/KeyboardManager.hpp
  - src/managers/KeyboardManager.cpp
  - src/controllers/EventController.cpp
  - src/main.cpp
  - src/CMakeLists.txt
related_docs:
  - ../reference/event-flow.md
  - ../reference/interfaces.md
keywords: [manager, EventManager, ManagerOf, events, handleEvent, register, dispatch]
last_reviewed: 2026-08-01
---

# Add a new event manager

Goal: handle a category of SFML events (e.g. joystick) that currently maps to a `ManagerOf` with no installed manager.

```
- [ ] Ensure the event type maps to a ManagerOf in getManagerOf
- [ ] Specialize EventManager<ManagerOf::X> with an *I alias (interface)
- [ ] Implement the concrete manager (handleEvent + register methods)
- [ ] Add the .cpp to gameLib in src/CMakeLists.txt
- [ ] emplace it in main.cpp
- [ ] Add unit tests + a mock under tests/
```

## 1. Confirm the routing

`getManagerOf` in `src/managers/ManagerOf.cpp` classifies the event variant with `event.is<sf::Event::Xxx>()` checks and returns a `ManagerOf`. If your event subtypes already map to the target enum value, no change is needed; otherwise extend the matching `if`. Add a new enum value in `ManagerOf.hpp` only if introducing a genuinely new category.

## 2. Declare the interface

Specialize the `EventManager` template and expose registration methods, mirroring `src/managers/KeyboardManagerI.hpp`:

```cpp
template<>
struct EventManager<ManagerOf::Joystick> : public EventManagerI
{
    static constexpr ManagerOf MANAGER_TYPE = ManagerOf::Joystick;
    // using XHandler = std::function<...>;
    // virtual XUnRegisterer registerXHandler(XHandler&&) = 0;
};
using JoystickManagerI = EventManager<ManagerOf::Joystick>;
```

The static `MANAGER_TYPE` is required by the `IsBaseOfEventManager` concept used by `emplace`.

## 3. Implement the manager

Follow `KeyboardManager.hpp/.cpp`: store handlers in `ManagedList`s, decode the concrete sub-event in `handleEvent`, and return an `UnRegisterer` from each register method.

## 4. Register in the build and wire it up

- Add `managers/YourManager.cpp` to `gameLib` in `src/CMakeLists.txt`.
- In `src/main.cpp`, install it: `eventController->emplace<YourManager>(/* deps */);` (throws if already present).

## 5. Verify

Add a suite under `tests/unit_tests/managers/` and a mock under `tests/mocks/managers/`, register with `add_unit_test(...)`, then:

```sh
cmake --build --preset debug --target build_ut
ctest --preset debug
```

Background on dispatch: [event-flow.md](../reference/event-flow.md).
