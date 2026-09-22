---
title: SFML 3.1 overview and platforms
diataxis: explanation
audience: [ai, human]
related_code:
  - cmake/FetchSFML.cmake
  - src/window/WindowSFML.cpp
related_docs:
  - ./index.md
  - ./events.md
  - ./game-architecture.md
  - ../../tutorials/getting-started.md
keywords: [SFML, modules, platforms, Windows, Linux, macOS, Android, iOS, C++17, HarfBuzz, networking]
last_reviewed: 2026-09-22
---

# SFML 3.1 overview and platforms

## What SFML is

[SFML](https://www.sfml-dev.org/) (Simple and Fast Multimedia Library) is a portable, object-oriented C++ API for multimedia and games. Think of it as a higher-level, C++-friendly counterpart to SDL: windowing, input, 2D drawing, audio, and networking — not a full game engine.

You can use only the window module as an OpenGL context host, or stack Graphics / Audio / Network for a complete 2D game. SFML deliberately stays small so *you* own game architecture (screens, entities, physics, save data).

## Modules and dependencies

| Module | Role | Depends on |
|--------|------|------------|
| **System** | Time (`sf::Clock`, `sf::Time`), vectors, strings, threading helpers | — |
| **Window** | Window creation, events, keyboard/mouse/joystick, OpenGL context | System |
| **Graphics** | Sprites, shapes, text, textures, views, render targets | Window + System |
| **Audio** | Sound buffers, music streams, playback devices | System |
| **Network** | Sockets, HTTP(S), DNS, SFTP, packet helpers | System |

Link only what you use. With GCC, link order matters for static builds (dependents before dependencies is the usual rule; prefer CMake imported targets `SFML::Graphics`, etc.).

SFML 3 raised the language baseline to **C++17** (MSVC 16 / VS 2019, GCC 9, Clang 9, AppleClang 12 as documented minima). This project builds with **C++23** and fetches SFML with audio and network disabled (`SFML_BUILD_AUDIO` / `SFML_BUILD_NETWORK` OFF).

## Platforms

### Desktop (primary)

Official FAQ: SFML is available and fully functional on **Windows**, **Linux**, and **macOS**, 32- and 64-bit. Prebuilt SDKs ship for common toolchains (e.g. MSVC, MinGW, Clang on macOS).

- **Windows / macOS:** packaged builds typically vendor third-party deps so compile-from-SDK “just works.”
- **Linux:** you install system packages (OpenGL, X11/Xrandr/Xcursor/Xi, udev, FreeType, FLAC, Vorbis, …). Exact package names vary by distro. SFML 3 also needs `libxi-dev` when using the X11 backend (raw mouse input).

This repository targets desktop via CMake presets (`debug` / `release`) and FetchContent; see [getting-started.md](../../tutorials/getting-started.md).

### Mobile (supported, different packaging)

iOS and Android support has existed since SFML 2.2 and is considered much more stable today. SFML **3.1** shipped many mobile fixes and features, including:

- Android: static libraries, joystick support, 16 KB page sizes, surface recreate on backgrounding, scan codes, TextEntered backspace fixes
- iOS: orientation via `viewWillTransitionToSize`, resized events on window create, CMake/static-package fixes, improved examples

Mobile apps are not “drop a `.exe` next to resources.” Expect Gradle + Android Studio (native activity) or Xcode, and often building SFML as part of the app CMake tree. Official tutorials cover [Android](https://www.sfml-dev.org/tutorials/3.1/getting-started/android/) and iOS. **business-game does not currently target mobile.**

### What SFML does not provide

- A scene graph, ECS, or scripting runtime
- Built-in physics or collision resolution
- A first-class **3D** graphics API (team stance: stay 2D; you may mix raw OpenGL or another 3D engine for 3D)
- Automatic asset pipelines, UI frameworks, or level editors

Those are application concerns — which is why this repo layers controllers, screens, and entities on top of SFML.

## Highlights of SFML 3.1 (vs 3.0)

Released as tag **3.1.0** (2026-04). Feature highlights from the [release notes](https://github.com/SFML/SFML/releases/tag/3.1.0):

**Text / Unicode**

- Text shaping with **HarfBuzz**, bidirectional layout with **SheenBidi**, Unicode tables via cpp-unicodelib (Unicode 17.0)
- Complex scripts, ligatures, RTL/BiDi without manual glyph hacks
- Richer glyph-level info (`getShapedGlyphs()`, etc.); some older APIs such as certain `getKerning` / `findCharacterPos` overloads are deprecated in favor of shaping APIs

**Network**

- **TLS / HTTPS**, **IPv6**, improved DNS client
- **SFTP** client; classic FTP deprecated
- SocketSelector scalability improvements

**Graphics / system**

- **QOI** image format
- Runtime library version via `sf::version()`
- `sf::String` accepts `std::string_view`
- Range-based looping of `sf::VertexArray`
- Broader Unicode path handling for fonts/images/audio files

**Audio**

- Better playback-device / stream rerouting
- `sf::PlaybackDevice::getDeviceSampleRate`

**Window / events**

- Non-const `Event::getIf` / `Event::visit`
- Stronger checks on `WindowBase::handleEvents`
- Mobile input and window lifecycle fixes (see above)

New third-party pieces pulled in for 3.1 include HarfBuzz, SheenBidi, Mbed TLS, libssh2, and QOI (among others). FetchContent builds pull these as SFML’s CMake dictates — you usually do not wire them by hand when depending on SFML targets.

Migration notes: [3.0 → 3.1](https://www.sfml-dev.org/tutorials/3.1/migration/sfml-3.0/) and the larger [2 → 3](https://www.sfml-dev.org/tutorials/3.1/migration/sfml-2/) guide (vector2 APIs, scoped enums, optional `pollEvent`, CMake `SFML::` targets, …).

## In business-game

| Area | Status |
|------|--------|
| SFML version | `GIT_TAG 3.1.0` in [cmake/FetchSFML.cmake](../../../cmake/FetchSFML.cmake) |
| Modules linked | System (transitive), Window, Graphics — via `WindowSFML` / `gameLib` |
| Audio / Network | Not used yet |
| Text | Fonts through `ResourceManager`; HUD/buttons use `sf::Text` (benefits from 3.1 shaping when needed) |
| Platforms exercised | Desktop Windows/Linux-style CMake presets; no Android/iOS project files |

Next: [Events](./events.md).
