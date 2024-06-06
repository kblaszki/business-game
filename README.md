# 2D Board Game

This is a 2D board game developed using the SFML library.

Game name suggestions:
 - Empire Legends - Capital Pursuit | namespace elcp

## Table of Contents

- [Installation](#installation)
- [Building the Project](#building-the-project)
- [Running the Game](#running-the-game)
- [Running the Tests](#running-the-tests)
- [Contributing](#contributing)
- [License](#license)

## Installation

### Prerequisites

- [CMake](https://cmake.org/) (version 3.20 or higher)
- [SFML](https://www.sfml-dev.org/)
- A C++ compiler that supports C++23

```sh
git clone git@github.com:DevKrystian/business-game.git
cd business-game
```

## Building the Project

The project uses CMakePresets.json for managing build configurations.

### Available Presets
- release - Builds the project in release mode.
- debug - Builds the project in debug mode.

### Build Steps
Create a build directory:

1. Configure the project using one of the presets. For example, to use the release preset:

```sh
cmake --preset release
```
For the debug preset:
```sh
cmake --preset debug
```
2. Build the project:

```sh
cd ./build/release || cd ./build/debug
cmake --build .
```

## Running the Game
After building the project, you can run the game executable located in the build directory. The exact path will depend on your build configuration. For example:

```sh
./build/release/bin/game.exe
```
### Running the Tests
Tests are built only in debug mode. To run the tests:

Build the project with the debug preset as described in the Building the Project section.

Run the tests executable located in the build directory. For example:

```sh
./build/debug/bin/some_tests.exe
```
## Contributing
Contributions are welcome! Please follow these steps to contribute:

Fork the repository.
Create a new branch for your feature or bugfix.
Commit your changes to the new branch.
Push the branch to your forked repository.
Open a pull request with a description of your changes.

## License
This project is licensed under the MIT License. See the [LICENSE](https://opensource.org/license/mit) file for details.
