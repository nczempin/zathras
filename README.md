# zathras

I wanted to get more seriously into more modern C++, bitboards, and eventually a multithreaded chess engine.

So I got started on Zathras, named after my favourite character from the Babylon 5 universe.

I only got as far as using modern C++ async I/O capabilities, but making it truly multithreaded would eventually have made the "Zathras" name very fitting.

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

## Project status

Zathras began as an experiment in modern C++ and bitboard-based chess programming. The long-term goal is a multithreaded engine, but development is currently incomplete and focuses on the core UCI functionality and basic search features.

