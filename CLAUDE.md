# CLAUDE.md - Zathras Chess Engine

This file provides guidance to Claude Code (claude.ai/code) when working with the Zathras chess engine codebase.

## Repository Overview

**Zathras** is a chess engine written in C++17 that implements the UCI (Universal Chess Interface) protocol. The project focuses on correctness and educational value over performance optimization.

**Technology Stack**: C++17, Make build system, UCI protocol  
**Current Version**: 0.0.5 baseline (stable)  
**Working Directory**: `/home/nczempin/git/ai/nc-aad/zathras/`  
**Platform**: Linux (WSL2)

## Project Status and Branch Strategy

### Current Stable Baseline: Version 0.0.5

**IMPORTANT**: As of June 2025, the project uses commit `be3689e` ("Tweak some minor things, bump version to 0.0.5") as the stable foundation for all development.

**Branch Strategy**:
- `main`: Production-ready releases (currently at 0.0.5 baseline)
- `dev`: Default development branch (currently at 0.0.5 baseline) 
- `feature/ZTH-*`: Feature branches following git flow (PROJECT_ID: ZTH)

**Repository Default Branch**: `dev` (set in GitHub)

### Historical Context - AVOID POST-0.0.6 BRANCHES

**Critical**: Any branches that diverge toward or beyond version 0.0.6 should be IGNORED. These represent failed optimization attempts that introduced bugs and instability.

**Archived Branches** (for reference only):
- `develop-archive`: Old unstable development branch
- `main-archive`: Old unstable main branch  
- `rebuild-v0.1.0`: Incorrectly based on 0.0.3, not 0.0.5

**Branch Cleanup Status**: Archive/obsolete branches need cleanup and GitHub issue tracking.

## Build System

### Key Commands

```bash
# Build the engine
make clean && make

# Test basic functionality  
echo -e "uci\nquit" | ./zathras

# Run perft tests
echo -e "uci\nposition startpos\nperft 4\nquit" | ./zathras
```

### Makefile Configuration

The Makefile is configured for the library-based architecture:

```makefile
CC=g++
CFLAGS=-Ofast -g -std=c++17 -Wall -Wextra -pedantic -flto -fno-builtin -m64 -malign-data=cacheline -march=sandybridge -I./zathras_lib/src
BIN=zathras
SRC=$(wildcard src/*.cpp)
LIB_SRC=$(wildcard zathras_lib/src/*.cpp)  
LIB_OBJ=$(LIB_SRC:.cpp=.o)
CPP_OBJ=$(SRC:.cpp=.o)
OBJ=$(CPP_OBJ) $(LIB_OBJ)
```

**Important**: The build system includes both `src/` and `zathras_lib/src/` source compilation with proper include paths.

## Codebase Architecture

### Directory Structure
```
src/                    # Main UCI interface and application code
zathras_lib/src/        # Core chess engine library
  ├── Position.cpp      # Board representation and game state
  ├── Move_generator.cpp # Legal move generation
  ├── Searcher.cpp      # Search algorithm (alpha-beta)  
  ├── Square.cpp        # Square and bitboard utilities
  └── *.h              # Header files
tests/                  # Test suites
.github/workflows/      # CI/CD pipeline
```

### Core Components
- **UCI Interface** (`src/`): Handles Universal Chess Interface protocol
- **Position Management** (`zathras_lib/src/Position.*`): Board state, FEN parsing, move making/unmaking
- **Move Generation** (`zathras_lib/src/Move_generator.*`): Legal move generation with bitboards
- **Search Engine** (`zathras_lib/src/Searcher.*`): Alpha-beta search with iterative deepening
- **Bitboard System** (`zathras_lib/src/Square.*`): Square manipulation and bitboard operations

## Testing and Validation

### Perft Testing (Position Permutation Testing)
The engine uses perft testing to validate move generation correctness:

**Expected Results** (starting position):
- Perft 1: 20 moves
- Perft 2: 400 moves  
- Perft 3: 8,902 moves
- Perft 4: 197,281 moves
- Perft 5: 4,865,609 moves

### CI/CD Pipeline
Located in `.github/workflows/ci.yml`:
- Automated build testing on push/PR
- Perft validation up to depth 5
- Performance benchmarking
- Artifact upload on failure

**Trigger Branches**: `main`, `develop`, `feature/**`, `rebuild-v0.1.0`

## Development Workflow

### Git Flow Pattern
1. **Feature Development**: Create `feature/ZTH-{issue}-{title}` branches from `dev`
2. **Testing**: Ensure perft tests pass before merge
3. **Integration**: Merge feature branches back to `dev` via PR
4. **Releases**: Merge `dev` to `main` for stable releases

### Branch Naming Convention
- **Feature branches**: `feature/ZTH-{issue-number}_{Description}`
- **PROJECT ID**: ZTH (Zathras)
- **Target branch**: All feature PRs should target `dev`, not `main`
- **One feature per branch**: Don't mix multiple issues in a single branch

### Before Starting Work
1. **Always work from `dev` branch** - this is the stable 0.0.5 baseline
2. **Never use post-0.0.6 branches** as reference or merge source
3. **Run perft tests** to validate correctness: `make && echo -e "uci\nposition startpos\nperft 4\nquit" | ./zathras`
4. **Check CI pipeline** status before pushing
5. **Run tests**: `make test` (when test framework is available)

### Common Development Tasks
```bash
# Start new feature from stable baseline
git checkout dev
git pull origin dev  
git checkout -b feature/ZTH-{issue}-{title}

# Build and test
make clean && make
echo -e "uci\nposition startpos\nperft 3\nquit" | ./zathras

# Debug move generation (when available)
echo -e "uci\nposition startpos\ndivide 3\nquit" | ./zathras

# Push feature branch
git push -u origin feature/ZTH-{issue}-{title}
```

### Development Commands
```bash
# Build system
make clean && make        # Clean build
make test                 # Run test suite (when available)

# Engine testing  
echo -e "uci\nquit" | ./zathras                                    # Test UCI interface
echo -e "uci\nposition startpos\nperft 3\nquit" | ./zathras        # Performance test
echo -e "uci\nposition startpos\ndivide 3\nquit" | ./zathras       # Debug move generation (when available)

# Git workflow
git checkout dev                              # Switch to dev branch
git checkout -b feature/ZTH-{issue}_{title}  # Create feature branch
git push -u origin feature/ZTH-{issue}_{title} # Push new branch
```

## Known Issues and Limitations

### Current Status: 0.0.5 Baseline
- ✅ **Build System**: Fixed library compilation issues
- ✅ **Basic UCI**: Protocol implementation working
- ✅ **Move Generation**: Perft tests passing through depth 4
- ✅ **CI Pipeline**: Automated testing infrastructure
- ⚠️ **Performance**: Focus is on correctness, not optimization
- ⚠️ **Feature Completeness**: Basic engine functionality only

### Stale Branches Requiring Issue Extraction

**IMPORTANT**: Before deleting stale branches, their improvements must be extracted as GitHub issues for future re-implementation from the 0.0.5 baseline.

**Stale Branches to Process**:
- `feature/ZA-54_Fix_assertion_failure_moving_not_0`: Contains assertion failure fixes
- `feature/ZA-57_Fix_move_generator_perft_discrepancy`: Move generator debugging improvements  
- `feature/ZA-60_Fix_test_compilation_errors`: Test framework fixes
- `feature/ZA-67_Fix_divide_command_output_format`: Divide command improvements
- `feature/ZA-68_Fix_Makefile_library_compilation_wildcard_issue`: Build system fixes (already applied)
- `develop-archive` / `main-archive`: Archive branches from unstable period
- `rebuild-v0.1.0`: Incorrectly based on 0.0.3 instead of 0.0.5

**Issues Created for Stale Branch Work**:
1. **#73: Re-implement assertion failure fixes for 0.0.5 baseline**
   - Based on work in ZA-54 branch
   - Needs to prevent invalid moves and king captures
   
2. **#74: Re-implement move generator perft validation**
   - Based on work in ZA-57 branch  
   - Systematic debugging of perft discrepancies
   
3. **#75: Re-implement test framework integration**
   - Based on work in ZA-60 branch
   - Catch2 integration and compilation fixes
   
4. **#76: Re-implement divide command for debugging**
   - Based on work in ZA-67 branch
   - Enhanced move generation debugging output

5. **#77: Clean up stale branches after work extraction**
   - Repository maintenance and cleanup
   - Remove obsolete branches safely

**Branch Cleanup Process**:
1. ✅ Extract intents as GitHub issues (#73, #74, #75, #76)
2. ✅ Validate issues cover all valuable work
3. ⏳ Delete stale local branches (tracked in #77)
4. ⏳ Delete stale remote branches (tracked in #77)
5. ⏳ Update this documentation

### Context Loss Prevention
**Documentation Strategy**: This CLAUDE.md file serves as the authoritative source for:
- Branch strategy decisions and rationale
- Build system configuration  
- Testing procedures and expected results
- Historical context about failed optimization attempts

**Update Requirement**: Any significant architectural changes or branch strategy modifications MUST be documented here to prevent context loss in future conversations.

## Notes for Claude Code

### Critical Reminders
- **NEVER use branches that diverge toward/beyond 0.0.6** - these are known bad paths
- **Always validate with perft testing** before considering changes successful
- **Document context-critical decisions** in this file to prevent repetition
- **Use git flow branching** with ZTH project ID for all feature work
- **Set GitHub issues** for any cleanup or systematic work to maintain traceability

### Development Priorities
1. **Stability over Performance**: Correctness is more important than optimization
2. **Systematic Approach**: Use GitHub issues and feature branches for all changes  
3. **Testing First**: Validate with perft before considering any changes complete
4. **Documentation**: Update this file when making architectural decisions

### Emergency Recovery
If context is lost and previous conversation state is unclear:
1. **Check current branch**: Should be on `feature/ZTH-*` or `dev`
2. **Verify 0.0.5 baseline**: `git log --oneline | grep "0.0.5"` should show `be3689e`
3. **Test build**: `make clean && make` should succeed
4. **Run perft**: `echo -e "uci\nposition startpos\nperft 3\nquit" | ./zathras` should show 8902
5. **Check this file**: Read CLAUDE.md for current project status and strategy