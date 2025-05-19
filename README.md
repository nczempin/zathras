[![CI](https://github.com/nczempin/zathras/actions/workflows/ci.yml/badge.svg)](https://github.com/nczempin/zathras/actions/workflows/ci.yml)

# Zathras

Zathras is a small chess engine written in modern C++. It began as an
exercise in async I/O and bitboards and may grow into a multithreaded
engine one day.

## Requirements

- Python 3
- C++17 compiler (`g++`, `clang++` or MSVC)
- `make` on Unix-like systems or Visual Studio on Windows
- `cppcheck` (optional for extra linting)

## Quickstart

From a fresh clone run the following command to build, test and create a
package archive:

```bash
python run.py
```

A `zathras.zip` archive will appear in the project root containing the
engine executable and static library.

## Manual Build

```bash
make        # build the engine and static library
make test   # run unit tests
make clean  # remove artifacts
```

The `zathras` binary appears in the repository root.

## Run

```bash
./zathras
```

The program prints a short greeting and waits for UCI commands like
`uci`, `isready`, and `quit`.

## Project Status

Development is exploratory and incomplete. Zathras currently offers only
basic UCI handling and search routines, with multithreading planned for
the future.
