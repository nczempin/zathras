[![CI](https://github.com/nczempin/zathras/actions/workflows/ci.yml/badge.svg)](https://github.com/nczempin/zathras/actions/workflows/ci.yml)

# zathras

I wanted to get more seriously into more modern C++, bitboards, and eventually a multithreaded chess engine.

So I got started on Zathras, named after my favourite character from the Babylon 5 universe.

I only got as far as using modern C++ async I/O capabilities, but making it truly multithreaded would eventually have made the "Zathras" name very fitting.


## Running tests

The project uses [Catch2](https://github.com/catchorg/Catch2) for unit testing.
The single-header distribution is already included under
`tests/external/catch_amalgamated.hpp`, so no additional downloads are
required.

To build and run tests with a C++17 compiler, compile your test sources and
include the `tests` directory so that the Catch2 header can be found. For
example:

```bash
g++ -std=c++17 -Isrc -Itests -o my_tests path/to/my_tests.cpp
./my_tests
```

## Prerequisites

- **Operating system:** A Unix-like environment (tested on Ubuntu) or any system with a recent C++17 toolchain.
- **Compiler:** `g++` or `clang++` with C++17 support (the Makefile uses `g++`).
- **Build tools:** `make` and standard development utilities.

## Building

Run the following commands from the repository root:

```bash
# build the static library and the engine binary
make

# remove artifacts
make clean
```

This produces the `zathras` binary in the project root directory.

## Running

After `make` finishes, execute:

```bash
./zathras
```

The program displays a welcome message and waits for UCI commands on standard input. Enter commands such as `uci`, `isready` or `quit` to interact with the engine.

## Comparing perft with Stockfish

The repository includes a small helper script named `stock.sh` for validating
perft results against the [Stockfish](https://stockfishchess.org/) engine. The
script sends a FEN position followed by a `perft` command. Ensure that the
`stockfish` binary is available in your `PATH` and run:

```bash
./stock.sh
```

Stockfish will output the node count for `perft 6`, which can be compared
against Zathras' own perft implementation.

## Architecture Overview

Zathras is designed with modern C++ principles and focuses on:

- **Bitboard representation**: Efficient board state using 64-bit integers
- **Modern C++ features**: Leveraging C++17 capabilities for cleaner code
- **Async I/O**: Non-blocking UCI communication for better responsiveness
- **Modular design**: Clear separation between engine logic and UCI interface

### Key Components

- `src/`: Main engine application and UCI interface
- `zathras_lib/`: Core chess logic as a reusable library
- `tests/`: Comprehensive test suite using Catch2
- `msvc/`: Windows Visual Studio project files

## Development Roadmap

Zathras is actively developed with these planned features:

| Feature | Status | Priority |
|---------|--------|----------|
| UCI protocol basics | ✅ Complete | - |
| Move generation & validation | ✅ Complete | - |
| Bitboard operations | ✅ Complete | - |
| Basic search (minimax/alpha-beta) | 🚧 In Progress | High |
| Position evaluation | 🚧 In Progress | High |
| Transposition tables | 📋 Planned | Medium |
| Multithreaded search | 📋 Planned | Medium |
| Advanced pruning techniques | 📋 Planned | Low |

## Project status

Zathras is a semi-actively developed modern C++ chess engine exploring advanced techniques. While functional for basic UCI operations and move generation, search and evaluation components are still being refined.


