# SFML 2.6.1 → 3.1.0 migration plan

SFML 3 requires C++17 (repo is C++20 — fine) and rewrites the event API around `std::variant`/`std::optional`. In this codebase the blast radius is small and well-contained: `cmake/FetchSFML.cmake`, `src/window/`, `src/managers/`, `src/controllers/EventController.*`, `src/entities/Paddle.cpp` + `Button.cpp`, `src/main.cpp`, and the tests/mocks. Reference: [official migration guide](https://www.sfml-dev.org/tutorials/3.1/migration/sfml-2/).

Work through the steps in order; the build stays broken between steps 2 and 8, so do the migration on a branch and commit once green.

## Master checklist

```
- [ ] 1. CMake: SFML 3.1.0 + new target names (+ FetchContent_MakeAvailable)
- [ ] 2. EventCollectorI + WindowSFML: optional-returning pollEvent, VideoMode
- [ ] 3. EventController + getManagerOf: variant-based dispatch
- [ ] 4. KeyboardManager(I): event subtypes + scoped enums
- [ ] 5. MouseManager(I): event subtypes
- [ ] 6. GameExitManager: is<Closed>
- [ ] 7. Entities: Paddle (vectors, scoped keys), Button (font/text/rect API)
- [ ] 8. main.cpp + EventPrinter
- [ ] 9. Tests and mocks
- [ ] 10. CI flags cleanup
- [ ] 11. Verify: build, ctest, manual smoke run
```

## 1. CMake

In `cmake/FetchSFML.cmake`: tag `2.6.1` → `3.1.0`, and replace the deprecated `FetchContent_Populate` pattern with `FetchContent_MakeAvailable` (see [stack-review.md](stack-review.md)). SFML 3 builds static by default — that is the desired end state anyway.

In `src/CMakeLists.txt`, target names changed:

```cmake
# before
target_link_libraries(gameLib PUBLIC sfml-graphics sfml-system sfml-window)
# after
target_link_libraries(gameLib PUBLIC SFML::Graphics SFML::System SFML::Window)
```

## 2. Window layer

`sf::WindowBase::pollEvent` now returns `std::optional<sf::Event>`; `sf::VideoMode` takes a `sf::Vector2u`.

`src/window/EventCollectorI.hpp`:

```cpp
// before
virtual bool pollEvent(sf::Event& event) = 0;
// after
virtual std::optional<sf::Event> pollEvent() = 0;
```

`src/window/WindowSFML.cpp`:

```cpp
// before
window{sf::VideoMode(1280, 720), "Business game", sf::Style::Default}
// after
window{sf::VideoMode({1280, 720}), "Business game", sf::Style::Default}
```

and forward the new `pollEvent` signature. `clear`, `draw`, `display`, `close`, `isOpen` are unchanged.

## 3. Event dispatch (the structural change)

`sf::Event::EventType` and `event.type` no longer exist; events are a variant queried with `event.is<T>()` / `event.getIf<T>()`. Rework `getManagerOf` to take the whole event (`src/managers/ManagerOf.hpp/.cpp`):

```cpp
// before
ManagerOf getManagerOf(sf::Event::EventType event);
// after
ManagerOf getManagerOf(const sf::Event& event);
```

Implementation maps subtypes: `is<sf::Event::Closed>()` → `GameExit`; `is<sf::Event::Resized>()` / `FocusLost` / `FocusGained` → `GameWindow`; `KeyPressed`/`KeyReleased`/`TextEntered` → `Keyboard`; the mouse subtypes → `Mouse`; joystick/touch/sensor likewise; otherwise `None`.

`EventController::handleEvents` (`src/controllers/EventController.cpp`):

```cpp
// before
sf::Event event{};
while(eventCollector.pollEvent(event)) { ... getManagerOf(event.type) ... }
// after
while(const auto event = eventCollector.pollEvent())
{
    auto eventManager = eventManagers.find(getManagerOf(*event));
    if(eventManagers.end() not_eq eventManager)
    {
        eventManager->second->handleEvent(*event);
    }
}
```

`EventManagerI::handleEvent(const sf::Event&)` keeps its signature — only the manager internals change.

## 4. KeyboardManager

`sf::Event::KeyEvent` is gone; the subtypes are `sf::Event::KeyPressed` / `sf::Event::KeyReleased` (same fields: `code`, `scancode`, `alt`, ...). `sf::Keyboard::Key` is scoped.

In `src/managers/KeyboardManagerI.hpp`, redefine the handler payload. Simplest mapping that preserves the current API shape: keep `KeyStatus` and pass the key data as a single struct — SFML 3's `KeyPressed`/`KeyReleased` carry identical members, so pick one canonical type or introduce a small local struct:

```cpp
using KeyHandler = std::function<void(KeyStatus, const sf::Event::KeyPressed&)>; // canonical payload
```

In `KeyboardManager::handleEvent`, decode with `getIf`:

```cpp
if(const auto* key = event.getIf<sf::Event::KeyPressed>())
    handleKeyEvent(KeyStatus::Pressed, *key);
else if(const auto* key = event.getIf<sf::Event::KeyReleased>())
    handleKeyEvent(KeyStatus::Released, /* map to canonical payload */);
else if(const auto* text = event.getIf<sf::Event::TextEntered>())
    handleTextEvent(*text);
```

(`KeyReleased` → canonical payload is a member-by-member copy; keep it in one helper.)

## 5. MouseManager

Same pattern: `sf::Event::MouseMoveEvent`/`MouseButtonEvent`/`MouseWheelScrollEvent` become subtypes `sf::Event::MouseMoved`, `MouseButtonPressed`, `MouseButtonReleased`, `MouseWheelScrolled`, `MouseEntered`, `MouseLeft`. Positions are `sf::Vector2i position` instead of `int x, y` — this ripples into `OnClickHandler`/`OnHoverHandler` (`event.x` → `event.position.x`) and `IsHoverHandlerI::isHover(int, int)` which should become `isHover(sf::Vector2i)`.

## 6. GameExitManager

`handleEvent` currently assumes it only receives close events (routing guarantees it). Verify with `event.is<sf::Event::Closed>()` for clarity; the rest (`close()`, exit handlers) is SFML-independent.

## 7. Entities

`src/entities/Paddle.cpp`:

```cpp
// before
shape.setPosition(400, 500);
shape.move(-10.f, 0.f);
sf::Keyboard::Left
// after
shape.setPosition({400, 500});
shape.move({-10.f, 0.f});
sf::Keyboard::Key::Left
```

`src/entities/Button.cpp` / `Button.hpp` (most changes in one class):

- `font.loadFromFile(...)` → `font.openFromFile(...)`
- `sf::Text` has no default constructor — construct with the font: `sf::Text buttonText{font, text, 24}`. Mind member declaration order: `font` must be declared before `buttonText`.
- `sf::FloatRect`: `left/top/width/height` → `.position` / `.size`:

```cpp
// before
buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
// after
buttonText.setOrigin(textRect.position + textRect.size / 2.0f);
```

- `getGlobalBounds().contains(x, y)` → `contains(sf::Vector2f{position})`.
- `setPosition`/`setOrigin` take `sf::Vector2f` (single argument).

## 8. main.cpp and EventPrinter

- `sf::Keyboard::Escape` → `sf::Keyboard::Key::Escape`; the Escape handler's payload type follows step 4.
- `src/utils/EventPrinter.*` formats `event.type` — rewrite against subtypes or delete it together with the per-event logging (see architecture review issue 4; deleting is recommended).

## 9. Tests and mocks

- `tests/mocks/window/EventCollectorMock.hpp` (and `WindowMock`): new `pollEvent` signature.
- Manager mocks: new handler payload types.
- Unit tests constructing `sf::Event` values: build subtype values directly (e.g. `sf::Event event{sf::Event::KeyPressed{...}}`).
- Run the full suite; fix compilation before behavior.

## 10. CI

In `.github/workflows/ci.yml`: SFML 3 replaced OpenAL with miniaudio — drop `-DOPENAL_LIBRARY=... -DOPENAL_INCLUDE_DIR=...` from the Linux flags and the OpenAL apt packages/verification step; check whether `SFML_USE_MESA3D` is still honored (it is SFML's CI-oriented flag; keep if accepted).

## 11. Verification gate

```
- [ ] cmake --preset debug (fresh build dir, no CMP0169 warnings)
- [ ] cmake --build --preset debug --target game — zero warnings (repo builds with -Werror)
- [ ] cmake --build --preset debug --target build_ut && ctest --preset debug — all green
- [ ] Manual smoke: menu renders text, hover changes button color, Start switches screen, paddle moves, Escape and Exit close
- [ ] cmake --preset release && build game — release also green
- [ ] CI passes on both platforms
```
