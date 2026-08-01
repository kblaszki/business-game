# Stack review

Assessment date: 2026-08-01. Verdict: the language/tooling core is current; the two fetched dependencies and the fetch mechanism itself are outdated.

## Version matrix

| Component | In repo | Current (2026-08) | Status | Recommendation |
|-----------|---------|-------------------|--------|----------------|
| C++ standard | C++20 | C++23 available | OK | Keep C++20; nothing in the codebase needs C++23 |
| CMake minimum | 3.20 | 4.x installed locally | OK | Keep 3.20 as minimum; fix deprecations below |
| Ninja presets | debug / release | — | OK | Keep |
| SFML | 2.6.1 | **3.1.0** (2026-04-16) | **Outdated** | Migrate — see [sfml3-migration.md](sfml3-migration.md) |
| GoogleTest | v1.14.0 | **v1.17.0** (2025-04-30) | Outdated | Bump tag; requires C++17 (we have C++20) |
| clang-format / clang-tidy | configs at root | — | OK | Keep |
| CI | Windows MinGW GCC 12.2 + Linux GCC | — | OK | GCC 12.2 covers C++17 required by SFML 3 |

## Why migrate SFML 2.6 → 3.1

- SFML 2.x is the legacy line; 3.x is where fixes and features land (3.0 released 2024-12, 3.1 in 2026-04 with a new text engine, TLS/HTTPS networking, audio via miniaudio).
- The 3.x event API (`std::variant` + `std::optional`) is type-safe and maps naturally onto this project's manager dispatch.
- The cost is contained: this codebase touches SFML in a small number of files (window, managers, two entities, `main.cpp`), thanks to the interface layer.
- Requirement is only C++17; the project already builds with C++20.

## Build system issues (independent of SFML version)

### 1. Deprecated `FetchContent_Populate`

`cmake/FetchSFML.cmake` and `cmake/FetchGTest.cmake` use the `FetchContent_Populate` + manual `add_subdirectory` pattern, deprecated by CMake policy CMP0169 (warns on every configure today, will hard-fail in a future CMake).

```
- [x] Rewrite cmake/FetchSFML.cmake to FetchContent_MakeAvailable(sfml)
- [x] Rewrite cmake/FetchGTest.cmake the same way (plus INSTALL_GTEST OFF)
- [x] Stayed at cmake_minimum_required 3.20 (no EXCLUDE_FROM_ALL in Declare; CI runs CMake 3.22/3.25)
- [x] Reconfigure and confirm the CMP0169 warnings are gone
```

### 2. `gameLib` is SHARED without export annotations

`src/CMakeLists.txt` builds `gameLib` as `SHARED` with no `__declspec(dllexport)` / visibility macros and no `WINDOWS_EXPORT_ALL_SYMBOLS`. It links today only because MinGW GCC exports everything by default — MSVC would fail to produce usable imports. A shared library buys nothing here (single executable consumer).

```
- [x] Change add_library(gameLib SHARED ...) to STATIC in src/CMakeLists.txt
- [x] Rebuild game and tests; confirm no runtime DLL copying is needed anymore
```

### 3. GoogleTest bump

```
- [x] Change GIT_TAG in cmake/FetchGTest.cmake from v1.14.0 to v1.17.0
- [x] Rebuild build_ut, run ctest --preset debug, fix any deprecation fallout (none occurred)
```

## Explicitly fine as-is

- Warnings-as-errors flags, presets layout, resources copy step, tests-only-in-Debug split.
- CI matrix (Windows MinGW + Linux GCC); revisit flags only during the SFML 3 migration (`SFML_USE_MESA3D`, OpenAL flags become obsolete — SFML 3 dropped OpenAL for miniaudio).
