# business-game

A C++23 [SFML](https://www.sfml-dev.org/) 2D playable slice: main menu, a sandbox dummy, pause overlay, CMake presets, and windowless unit tests. Product-domain gameplay is not implemented yet.

## Table of Contents

- [Installation](#installation)
- [Project structure](#project-structure)
- [Building the Project](#building-the-project)
- [Running the Game](#running-the-game)
- [Running the Tests](#running-the-tests)
- [Contributing](#contributing)
- [License](#license)

## Installation

### Prerequisites

- [CMake](https://cmake.org/) 3.20 or higher
- [Ninja](https://ninja-build.org/) (used by CMake presets)
- A C++23 compiler (GCC, Clang, or MSVC)

SFML 3.1 is downloaded automatically via CMake FetchContent (`cmake/FetchSFML.cmake`; Audio and Network modules are off). A separate SFML install is not required.

```sh
git clone git@github.com:DevKrystian/business-game.git
cd business-game
```

## Project structure

| Path | Role |
|------|------|
| `src/main.cpp` | Entry point; runs `Game` |
| `src/Game.hpp` / `Game.cpp` | Window, event pump, `ScreenStack`, `InputMapper` |
| `tests/unit_tests/` | Debug GoogleTest suites (windowless) |
| `docs/` | Diátaxis documentation |

Static library target: `gameLib`. Executable target: `game`.

## Building the Project

The project uses `CMakePresets.json` for configurations.

### Available presets

- `debug` — Debug build (tests and `format` target enabled)
- `release` — Release build

### Build steps

1. Configure:

```sh
cmake --preset debug
```

or:

```sh
cmake --preset release
```

2. Build the game:

```sh
cmake --build --preset debug --target game
```

or:

```sh
cmake --build --preset release --target game
```

## Running the Game

After building, run the executable from the preset output directory:

```sh
# Windows
./build/debug/bin/game.exe
./build/release/bin/game.exe

# Linux / macOS
./build/debug/bin/game
./build/release/bin/game
```

## Running the Tests

Tests are available only for the **debug** preset. Suites: `smoke_test`, `fixed_timestep_test`, `iscreen_dummy_test`, `screen_stack_test`, `screen_transition_test`, `input_mapper_test`, `pause_blocks_ticks_test`, `world_test`. See [docs/how-to/build-and-test.md](docs/how-to/build-and-test.md).

```sh
cmake --preset debug
cmake --build --preset debug --target build_ut
ctest --preset debug
```

## Contributing

Contributions are welcome. Please:

1. Fork the repository and create a feature branch.
2. Match existing C++ style (see `.clang-format`). With a debug configure: `cmake --build --preset debug --target format`.
3. Treat warnings as errors — keep the build clean under the project flags.
4. Add or update unit tests under `tests/unit_tests/` when changing `gameLib`.
5. Add every new `src/**/*.cpp` to `gameLib` in `src/CMakeLists.txt`.
6. Keep `docs/` current for files listed in a doc's `related_code` (see `docs/index.md`).
7. Open a pull request with a short description of the change.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
