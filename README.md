# sfml-game-lab

A lab of small C++23 games on a shared [SFML](https://www.sfml-dev.org/) 3.1 engine (2D now, 2.5D-ready). The first title is a three-stage breakout in `games/arkanoid/`: four power-ups, a pause overlay, CMake presets, and windowless unit tests. Later games, such as Tetris and other classics, will sit beside it under `games/`.

The engine lives in `engine/` (`namespace sgl`). Prospective notes stay in [`mvp/`](mvp/README.md) (`01`–`09`); what landed is [`mvp/10`](mvp/10-engine-progress.md). The contract is [`docs/explanation/architecture.md`](docs/explanation/architecture.md).

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

SFML 3.1 is downloaded automatically via CMake FetchContent (`cmake/FetchSFML.cmake`; Audio and Network modules are off). A separate SFML install is not required. GoogleTest 1.18 is fetched only for Debug.

```sh
git clone git@github.com:DevKrystian/sfml-game-lab.git
cd sfml-game-lab
```

## Project structure

| Path | Role |
|------|------|
| `engine/` | Engine modules (`sgl_core`, input, scene, loop, render, collision, resources, `sgl_sfml`) |
| `games/arkanoid/` | Headless sim, scenes, and the `arkanoid` executable |
| `assets/fonts/` | UI TTF (`ASSET_DIR`) |
| `tests/engine/`, `tests/arkanoid/` | Debug GoogleTest (no window, except event translation) |
| `docs/` | Diátaxis documentation |
| `mvp/` | Prospective design (`01`–`09`) and living progress (`10`) |

Executable target: `arkanoid`. Only `engine/sfml` and `games/arkanoid/main.cpp` include SFML.

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
cmake --build --preset debug --target arkanoid
```

or:

```sh
cmake --build --preset release --target arkanoid
```

## Running the Game

After building, run the executable from the preset output directory:

```sh
# Windows
./build/debug/bin/arkanoid.exe
./build/release/bin/arkanoid.exe

# Linux / macOS
./build/debug/bin/arkanoid
./build/release/bin/arkanoid
```

A 1280×720 window titled "Arkanoid" opens on a Start / Quit menu. Enter starts Stage1. Clearing a stage loads the next one. Escape pauses.

Build and run Tetris the same way with target `tetris` (`./build/debug/bin/tetris` or `tetris.exe` on Windows). A 1280×720 window titled "Tetris" opens on a Start / Quit menu; Enter starts a game (arrows move, Down soft-drops, Space hard-drops, Up/X and Z rotate, C/Shift hold). Escape or P pauses.

## Running the Tests

Tests are available only for the **debug** preset. See [docs/how-to/build-and-test.md](docs/how-to/build-and-test.md).

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
4. Add or update unit tests under `tests/engine/` or `tests/arkanoid/`.
5. List every new `.cpp` in that module's `CMakeLists.txt`.
6. Keep `docs/` current for files listed in a doc's `related_code` (see `docs/index.md`).
7. Open a pull request with a short description of the change.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
