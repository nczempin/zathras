[![CI](https://github.com/nczempin/zathras/actions/workflows/ci.yml/badge.svg)](https://github.com/nczempin/zathras/actions/workflows/ci.yml)

# Zathras

Zathras is a small chess engine written in modern C++. It began as an
exercise in async I/O and bitboards and may grow into a multithreaded
engine one day.

## Requirements

- C++17 compiler (`g++` or `clang++`)
- `make` and common build tools

## Setup

Install the required packages with:

```bash
./setup.sh
```

## Build

```bash
make        # build the engine and static library
make clean  # remove artifacts
```

For Windows development, Visual Studio project files are located in the
`msvc/` directory. Open `zathras.sln` from there.

The `zathras` binary appears in the repository root. Run unit tests with
`make test`.

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
