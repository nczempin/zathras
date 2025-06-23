# C++ Linting Guidelines for Zathras

This document describes the linting setup for the Zathras chess engine.

## Overview

We use two complementary linting tools:

1. **clang-tidy** - Enforces C++ Core Guidelines and modern C++ best practices
2. **cppcheck** - Static analysis for bug detection and undefined behavior

## Setup

### Installing Linting Tools

On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install clang-tidy cppcheck
```

Or run the setup script:
```bash
./scripts/setup_linting.sh
```

## Running Linters

### Quick Linting
```bash
make lint          # Run all linters (non-blocking)
make lint-cppcheck # Run only cppcheck
make lint-clang-tidy # Run only clang-tidy
```

### CI Mode (Strict)
```bash
make lint-ci       # Fail on any linting errors
```

## Configuration

### clang-tidy

Configuration is in `.clang-tidy`. Key checks enabled:

- **C++ Core Guidelines** (`cppcoreguidelines-*`)
- **Modernization** (`modernize-*`)
- **Performance** (`performance-*`)
- **Readability** (`readability-*`)
- **Bug-prone patterns** (`bugprone-*`)
- **Static analysis** (`clang-analyzer-*`)

Some checks are disabled to reduce noise:
- Magic numbers warnings
- Trailing return type enforcement
- Excessive identifier length restrictions

### cppcheck

cppcheck runs with all checks enabled (`--enable=all`) and C++17 standard.

## Common Issues and Fixes

### Performance Issues

1. **Pass by const reference**: Large objects should be passed by const reference
   ```cpp
   // Bad
   void setArguments(vector<string> arguments);
   
   // Good
   void setArguments(const vector<string>& arguments);
   ```

2. **Unused variables**: Remove or use `[[maybe_unused]]` attribute

### Style Issues

1. **Variable scope**: Reduce variable scope where possible
2. **Missing override**: Mark overriding virtual functions with `override`
3. **Unused private functions**: Remove or implement them

## Suppressing Warnings

When a warning is a false positive or intentional:

### Inline Suppression
```cpp
// cppcheck-suppress unusedFunction
static void intentionally_unused() { }
```

### NOLINT for clang-tidy
```cpp
int magic_number = 42; // NOLINT(readability-magic-numbers)
```

## Integration with CI

The CI pipeline runs cppcheck on every push and pull request. Currently set to `continue-on-error` to avoid blocking builds while we address existing issues.

## Future Improvements

1. Enable clang-tidy in CI once it's installed
2. Gradually fix existing warnings
3. Eventually make linting checks mandatory (remove `continue-on-error`)
4. Add clang-format for consistent code formatting