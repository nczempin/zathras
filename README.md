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

### Linux/macOS

Install the required packages with:

```bash
./setup.sh
```

This script installs build dependencies including:
- C++17 compatible compiler (g++ or clang++)
- Make and build tools
- Development headers

### Windows

For Windows development, Visual Studio project files are located in the
`msvc/` directory. Open `zathras.sln` in Visual Studio 2019 or later.

Required tools:
- Visual Studio 2019+ with C++17 support
- Windows SDK

## Build

### Unix-like Systems (Linux/macOS)

```bash
# Build the engine and static library
make

# Build and run tests
make test

# Clean build artifacts
make clean
```

Build targets:
- `make` or `make zathras` - Build the main engine binary
- `make libzathras.a` - Build the static library only
- `make test` - Build and run the test suite
- `make simple_test` - Build and run basic functionality tests

The `zathras` binary appears in the repository root after a successful build.

### Windows (Visual Studio)

1. Open `msvc/zathras.sln` in Visual Studio
2. Select your desired configuration (Debug/Release)
3. Build → Build Solution (Ctrl+Shift+B)

The executable will be in the appropriate configuration directory under `msvc/`.

## Testing

The project includes a comprehensive test suite covering:

- FEN parsing functionality
- Move generation correctness
- Bitboard operations
- Basic engine operations

```bash
# Run all tests
make test

# Run simple verification tests
make simple_test
```

Tests are built using a lightweight testing framework and validate core chess engine functionality.

## Usage

### Basic Engine Operation

```bash
./zathras
```

The engine implements the UCI (Universal Chess Interface) protocol. After starting, it prints a greeting and waits for UCI commands:

**Essential UCI Commands:**
- `uci` - Initialize UCI mode and display engine info
- `isready` - Check if engine is ready for commands
- `ucinewgame` - Start a new game
- `position startpos` - Set starting position
- `position fen <fen-string>` - Set position from FEN notation
- `go depth <n>` - Search to specified depth
- `quit` - Exit the engine

**Example Session:**
```
./zathras
Zathras by nczempin
uci
id name Zathras
id author nczempin
uciok
isready
readyok
position startpos
go depth 5
... (search results)
quit
```

### Advanced Features

**Perft (Performance Test):**
The engine includes perft commands for move generation verification:
```
position startpos
perft 5
```

**Position Analysis:**
```
position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1
go depth 6
```

## Project Status

| Feature                         | Status       |
|---------------------------------|--------------|
| UCI protocol basics             | Works        |
| Move generation & perft command | Works        |
| Search routines                 | Works        |
| Asynchronous search             | Experimental |
| Multithreading                  | Planned      |

## Architecture

The project is structured as follows:

```
zathras/
├── src/                    # Main engine source
│   ├── main.cpp           # Entry point and UCI loop
│   ├── Command*.cpp       # UCI command handling
│   └── Perft_command.cpp  # Performance testing
├── zathras_lib/src/       # Core chess library
│   ├── Position*.cpp      # Board representation and FEN
│   ├── Move_generator.cpp # Legal move generation
│   ├── Bitboard.cpp       # Bitboard operations
│   ├── Searcher.cpp       # Search algorithms
│   └── Square.cpp         # Square/coordinate utilities
├── tests/                 # Test suite
└── msvc/                  # Visual Studio project files
```

**Key Design Decisions:**
- Bitboard-based board representation for performance
- Separate library (`libzathras.a`) for reusable chess logic
- UCI protocol compliance for compatibility with chess GUIs
- Template-heavy design for compile-time optimization

## Contributing

We welcome contributions from chess engine developers! This project serves as a learning platform for advanced engine techniques.

### Development Workflow

1. **Fork and Clone**
   ```bash
   git clone https://github.com/your-username/zathras.git
   cd zathras
   ```

2. **Set up Development Environment**
   ```bash
   ./setup.sh  # Install dependencies
   make        # Verify build works
   make test   # Verify tests pass
   ```

3. **Create Feature Branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

4. **Development Guidelines**
   - Follow existing code style and conventions
   - Add tests for new functionality
   - Ensure all tests pass before submitting
   - Keep commits focused and atomic
   - Write clear commit messages

5. **Testing Your Changes**
   ```bash
   make clean && make        # Clean build
   make test                 # Run test suite
   ./zathras                 # Manual testing
   ```

6. **Submit Pull Request**
   - Push to your fork
   - Create PR against the `main` branch
   - Include description of changes and rationale
   - Ensure CI passes

### Contribution Areas

**Beginner Friendly:**
- Documentation improvements
- Additional test cases
- Code cleanup and refactoring
- Performance benchmarking

**Intermediate:**
- UCI protocol extensions
- Move ordering improvements
- Evaluation function enhancements
- Search algorithm optimizations

**Advanced:**
- Multithreading implementation
- Advanced pruning techniques
- Endgame tablebase integration
- NNUE evaluation integration

### Code Style

- Use modern C++17 features appropriately
- Prefer RAII and smart pointers
- Follow const-correctness principles
- Use descriptive variable and function names
- Include inline documentation for complex algorithms

### Performance Considerations

This engine prioritizes educational value while maintaining competitive performance:

- Profile before optimizing
- Prefer algorithmic improvements over micro-optimizations
- Maintain readability alongside performance
- Document performance-critical sections
- Use appropriate data structures (bitboards for position, vectors for move lists)

### Issue Reporting

When reporting bugs or requesting features:

1. Check existing issues first
2. Provide minimal reproduction case
3. Include system information (OS, compiler version)
4. For crashes, include stack trace if possible
5. For performance issues, include profiling data

### Questions and Discussion

- Open an issue for questions about implementation
- Use discussions for broader engine development topics
- Reference relevant chess programming resources when applicable

The goal is to maintain this as a high-quality educational resource while building a competitive chess engine. All skill levels are welcome!
>>>>>>> origin/main
