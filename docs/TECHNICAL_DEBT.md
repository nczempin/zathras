# Technical Debt Inventory

This document tracks known technical debt and code quality issues in the Zathras chess engine.

## Critical Issues (Blocking Development)

### 1. Move Generator Crashes
- **Issue**: Assertion failure `moving != 0` in Position::make_move
- **Impact**: Cannot run perft tests, cannot validate move generation
- **Location**: `zathras_lib/src/Position.cpp:577`
- **Tracked**: Issue #54

### 2. Test Infrastructure Broken
- **Issue**: Tests don't compile due to namespace mismatch
- **Impact**: Cannot run unit tests
- **Location**: `tests/test_main.cpp`
- **Tracked**: Issue #60

## High Priority (Fix After Critical)

### 1. Compiler Warnings
- Unused parameters throughout Move_generator.cpp
- Unused variables in Searcher.cpp
- Constructor initialization order warnings
- Sign comparison warnings
- **Impact**: Hides potential bugs, unprofessional

### 2. Makefile Issues
- Duplicate target definitions causing warnings
- Overlapping rules for test targets
- **Location**: `Makefile:35-53`

### 3. Missing Board Initialization
- Position constructor doesn't initialize board array
- Relies on FEN parsing to set values
- **Risk**: Undefined behavior with uninitialized memory

## Medium Priority (Code Quality)

### 1. Architecture Issues
- **Uci.h doing too much** (9.5KB file with mixed responsibilities)
- UCI protocol mixed with engine logic
- Command pattern over-engineered for simple needs
- Async complexity without working basics

### 2. Code Style Inconsistencies
- Mixed naming conventions (camelCase vs snake_case)
- Inconsistent use of namespaces
- TODO comments everywhere
- Dead code (commented out sections)

### 3. Modern C++ Missing
- Raw arrays instead of std::array
- Manual memory management
- Not using constexpr for constants
- Missing move semantics

## Low Priority (Future Improvements)

### 1. Performance
- Bitboard operations could be optimized
- Move generation allocates unnecessarily
- No hash tables implemented

### 2. Features
- Incomplete async search implementation
- No opening book
- Basic evaluation function
- No endgame tables

## Code Smells

1. **Magic Numbers**: Hardcoded values throughout
2. **Long Functions**: Move generator functions are too complex
3. **Poor Separation**: Engine core mixed with interface
4. **Weak Testing**: Minimal test coverage
5. **Documentation**: Many functions undocumented

## Refactoring Opportunities

After move generator is fixed:

1. **Extract UCI Interface**: Separate protocol handling from engine
2. **Simplify Position Class**: Too many responsibilities
3. **Modernize Move Generator**: Use templates, constexpr
4. **Remove Command Pattern**: Over-engineered for current needs
5. **Consistent Error Handling**: Mix of exceptions and error codes

## Technical Debt Metrics

- **Compiler Warnings**: ~25 warnings on build
- **Dead Code**: ~10% of codebase is commented out
- **Test Coverage**: <5% (tests don't even compile)
- **Cyclomatic Complexity**: Several functions >20

## Priority Order

1. Fix crashes and get basic functionality working
2. Fix test infrastructure
3. Clean up compiler warnings
4. Refactor architecture issues
5. Modernize C++ usage
6. Add missing features

The key principle: **Make it work first, then make it good**.