# Zathras Chess Engine

A UCI-compliant chess engine written in C++17.

## Development Setup

### Quick Start
```bash
# Install development tools (Ubuntu/Debian)
./scripts/setup_dev_env.sh

# Just check what's installed (don't install)
./scripts/setup_dev_env.sh --check-only
```

## Building

```bash
make clean
make
```

## Running

```bash
./zathras
```

The engine implements the UCI (Universal Chess Interface) protocol.

## Testing

### Basic UCI Test
```bash
echo -e "uci\nquit" | ./zathras
```

### Perft Testing
```bash
echo -e "uci\nposition startpos\nperft 4\nquit" | ./zathras
```

## Code Quality

### Linting
The project uses C++ linting tools to maintain code quality:

```bash
make lint          # Run all linters
make lint-cppcheck # Run cppcheck only
make lint-clang-tidy # Run clang-tidy only
```

See [docs/LINTING.md](docs/LINTING.md) for details.

### CI/CD
Continuous Integration runs on every push and pull request:
- Build verification
- Case sensitivity checks
- Perft validation
- Code linting

## Development

See [CLAUDE.md](CLAUDE.md) for development guidelines and project structure.

## License

This project is under development.