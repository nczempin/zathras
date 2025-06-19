[![CI](https://github.com/nczempin/zathras/actions/workflows/ci.yml/badge.svg)](https://github.com/nczempin/zathras/actions/workflows/ci.yml)

# Zathras

Zathras is a small chess engine written in modern C++. It began as an
exercise in async I/O and bitboards and may grow into a multithreaded
engine one day.

This repository is aimed at chess engine developers who have already
built one or two engines and want to explore more advanced techniques.
The code strives to be approachable while showcasing ideas used in
engine design.

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

⚠️ **Currently Under Repair** - The engine has critical bugs preventing basic functionality.

| Feature                         | Status       | Notes |
|---------------------------------|--------------|-------|
| UCI protocol basics             | Partial      | Basic commands work |
| Move generation & perft command | **Broken**   | Crashes on perft (Issue #54) |
| Search routines                 | Untestable   | Depends on move generation |
| Asynchronous search             | Experimental | Not priority |
| Multithreading                  | Planned      | Future work |

See [ROADMAP.md](docs/ROADMAP.md) for the recovery plan and [Issue #57](https://github.com/nczempin/zathras/issues/57) for progress tracking.
