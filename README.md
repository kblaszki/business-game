# business-game

A C++20 [SFML](https://www.sfml-dev.org/) 2D game prototype: menu screen and paddle gameplay.

Longer-term direction is a business / board-style game (working name ideas such as *Empire Legends* are optional only; there is no `elcp` namespace in the code yet).

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
- A C++20 compiler (GCC, Clang, or MSVC)

SFML 2.6 is downloaded automatically via CMake FetchContent (`cmake/FetchSFML.cmake`). A separate SFML install is not required.

```sh
git clone git@github.com:DevKrystian/business-game.git
cd business-game
```

## Project structure

| Path | Role |
|------|------|
| `src/main.cpp` | Entry point; wires window, managers, screens, game loop |
| `src/controllers/` | `GameController`, `EventController`, `ScreenController` |
| `src/managers/` | Keyboard, mouse, and game-exit event managers |
| `src/screens/` | `MenuScreen`, `GameScreen` |
| `src/entities/` | Drawable/updatable game objects (`EntityI`) |
| `src/handlers/` | Click / hover helpers |
| `src/window/` | Window interfaces and SFML implementation |
| `src/utils/` | Shared helpers |
| `tests/unit_tests/` | GoogleTest unit tests (Debug only) |
| `tests/mocks/` | gmock doubles |
| `resources/` | Fonts and other assets (copied next to the binary) |

Library target: `gameLib`. Executable target: `game`.

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

After building, run the executable from the preset output directory (resources are copied beside it):

```sh
# Windows
./build/debug/bin/game.exe
./build/release/bin/game.exe

# Linux / macOS
./build/debug/bin/game
./build/release/bin/game
```

## Running the Tests

Tests are available only for the **debug** preset.

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
4. For changes to controllers or managers, update or add unit tests under `tests/unit_tests/` and mocks under `tests/mocks/`.
5. Add every new `src/**/*.cpp` to `gameLib` in `src/CMakeLists.txt`.
6. Open a pull request with a short description of the change.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
