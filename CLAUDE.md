# CLAUDE.md - Zathras Chess Engine

This file provides guidance to Claude Code (claude.ai/code) when working with the Zathras chess engine codebase.

## Repository Overview

**Zathras** is a chess engine written in C++17 that implements the UCI (Universal Chess Interface) protocol. The project focuses on correctness and educational value over performance optimization.

**Technology Stack**: C++17, Make build system, UCI protocol  
**Current Version**: 0.5.2 baseline (stable)  
**Working Directory**: `/home/nczempin/git/zathras/`  
**Platform**: Linux (WSL2)

## Project Status and Branch Strategy

### Current Stable Baseline: Version 0.5.2

**IMPORTANT**: As of June 2025, the project uses dev branch (commit be5a972) as the stable foundation for all development, representing v0.5.2 with cleaned build system and fixed CI.

**Branch Strategy**:
- `main`: Production-ready releases 
- `dev`: Default development branch (currently v0.5.2 stable baseline)
- `feature/ZTH-*`: Feature branches following git flow (PROJECT_ID: ZTH)

**Repository Default Branch**: `dev` (set in GitHub)

## Current Work Status (June 2025)

### Recent Stabilization (v0.5.2)
- ✅ **Build System**: Now uses separate build/ directory for object files
- ✅ **CI Pipeline**: Consolidated into single comprehensive job
- ✅ **Perft Testing**: Limited to depths 2-4 as requested
- ✅ **Repository Cleanup**: Established clean v0.5.2 baseline

### Completed Work (2025-06-26)
- ✅ **Issue #105**: Implemented divide command for perft debugging
- ✅ **Issue #110**: Fixed divide command bug (was showing move count instead of perft)
- ✅ **Issue #98-104**: COMPLETED - Restored promotion move generation
  - ✅ Generates 4 moves per promotion (Queen, Rook, Bishop, Knight)
  - ✅ Correctly sets promoted pieces on the board
  - ✅ Fixed unmake_move to properly restore pawns after promotions
  - ✅ Move display shows promotion notation (e.g., "e7e8q")
  - ✅ UCI parsing handles promotion moves
  - ✅ All promotion tests pass (simple, edge files, Position 5)

### Known Issues
- 🐛 **Issue #109**: Position 4 perft 4 incorrect (422,598 instead of 422,333)
  - This is the ONLY confirmed perft failure
  - All promotion tests pass correctly
  - Needs investigation using divide command

## Build System

### Key Commands

```bash
# Build the engine (uses build/ directory)
make clean && make

# Test basic functionality  
echo -e "uci\nquit" | ./zathras

# Run perft tests
echo -e "uci\nposition startpos\nperft 4\nquit" | ./zathras
```

### Makefile Configuration

The Makefile uses separate build directory:

```makefile
BUILD_DIR=build
CPP_OBJ=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC))

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/src $(BUILD_DIR)/zathras_lib/src

$(BUILD_DIR)/%.o: %.cpp
	$(CC) -o $@ -c $(CFLAGS) $<
```

## Testing and Validation

### Expected Perft Results (after promotion fix)
- Simple promotion perft 2: 41 moves (position: 4k3/P7/8/8/8/8/8/4K3 w - - 0 1)
- Starting position perft 3: 8,902 moves
- Starting position perft 4: 197,281 moves

### CI/CD Pipeline
- Single comprehensive job (build + extended perft validation)
- Runs on push to dev branch
- Tests perft depths 2-4 as requested
- Includes linting and build artifact upload

## Development Workflow

### Git Flow Pattern
1. **Feature Development**: Create `feature/ZTH-{issue}-{title}` branches from `dev`
2. **Testing**: Ensure perft tests pass before merge
3. **Integration**: Merge feature branches back to `dev` 
4. **Releases**: Merge `dev` to `main` for stable releases

### Branch Naming Convention
- **Feature branches**: `feature/ZTH-{issue-number}-{description}`
- **PROJECT ID**: ZTH (Zathras)
- **Target branch**: All feature PRs should target `dev`

## Known Issues and Current Tasks

### Issue #98-104: Promotion Move Generation
**Status**: COMPLETED (2025-06-26)

**Completed**:
1. ✅ Restored Move_type enum with PROMOTION_QUEEN, PROMOTION_ROOK, PROMOTION_BISHOP, PROMOTION_KNIGHT
2. ✅ Fixed visit_pawn_caps() to generate 4 promotion moves  
3. ✅ Fixed visit_pawn_nocaps() to generate 4 promotion moves
4. ✅ Fixed make_move/unmake_move promotion handling (including pawn restoration bug)
5. ✅ Restored UCI promotion parsing
6. ✅ Fixed Move display to show promotion notation
7. ✅ All promotion tests pass including edge files

### Issue #109: Position 4 perft 4 incorrect
**Status**: Open - Only confirmed bug

**Problem**: Position 4 perft 4 returns 422,598 instead of 422,333 (+265 moves)
- All 6 legal moves show slightly higher counts than Stockfish
- Needs investigation using divide command to narrow down the issue

### Issue #97: CI Optimization
**Status**: Completed for now - consolidated CI into single job

## Development Commands
```bash
# Build system
make clean && make        # Clean build using build/ directory

# Engine testing  
echo -e "uci\nquit" | ./zathras                                    # Test UCI interface
echo -e "uci\nposition startpos\nperft 3\nquit" | ./zathras        # Performance test

# Git workflow
git checkout dev                                    # Switch to dev branch
git checkout -b feature/ZTH-{issue}-{title}        # Create feature branch
git push -u origin feature/ZTH-{issue}-{title}     # Push new branch
```

## Notes for Claude Code

### Critical Reminders
- **Work from dev branch** - this is the stable v0.5.2 baseline
- **Use proper ZTH-{issue} branch naming** with GitHub issue tracking
- **Validate with perft testing** before considering changes successful
- **Document significant changes** in this file
- **Build system uses build/ directory** - don't put object files in src/

### Development Priorities
1. **Fix Position 4 perft 4** (Issue #109) - only remaining perft failure
2. **Maintain v0.5.2 stability** - avoid breaking existing functionality
3. **Systematic approach** - use GitHub issues and proper git flow
4. **CI validation** - ensure all tests pass before merging

### Emergency Recovery
If context is lost:
1. **Check current branch**: Should be on `feature/ZTH-*` or `dev`
2. **Check build**: `make clean && make` should succeed
3. **Test basic perft**: Should get 8902 for perft 3 on starting position
4. **Read this file**: Check current work status and priorities