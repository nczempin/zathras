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

### Active Development
- 🔄 **Issue #98**: Restoring promotion move generation (4 moves per promotion)
- 🔄 **Feature Branch**: `feature/ZTH-98-restore-promotion-move-generation`

**Current Problem**: Promotion moves only generate 1 move instead of 4 (Queen, Rook, Bishop, Knight), causing perft test failures.

### Previous Work Found in Git History
Based on reflog analysis, promotion fixes were previously implemented but lost during cleanup:
- **f69c43e**: Added PROMOTION_* move types to enum
- **9496f97**: Fixed make_move/unmake_move promotion handling
- **Need to restore**: All promotion-related functionality

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
- Simple promotion perft 2: 400 moves
- Promotion-only perft 2: 64 moves
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

### Issue #98: Restore Promotion Move Generation
**Status**: In Progress on `feature/ZTH-98-restore-promotion-move-generation`

**Problem**: Each pawn promotion generates only 1 move instead of 4, causing perft failures.

**Required Fixes**:
1. ✅ Restore Move_type enum with PROMOTION_QUEEN, PROMOTION_ROOK, PROMOTION_BISHOP, PROMOTION_KNIGHT
2. 🔄 Fix visit_pawn_caps() to generate 4 promotion moves  
3. ⏳ Fix visit_pawn_nocaps() to generate 4 promotion moves
4. ⏳ Restore make_move/unmake_move promotion handling
5. ⏳ Restore UCI promotion parsing

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
1. **Fix promotion bug** (Issue #98) - critical for perft accuracy
2. **Maintain v0.5.2 stability** - avoid breaking existing functionality
3. **Systematic approach** - use GitHub issues and proper git flow
4. **CI validation** - ensure all tests pass before merging

### Emergency Recovery
If context is lost:
1. **Check current branch**: Should be on `feature/ZTH-*` or `dev`
2. **Check build**: `make clean && make` should succeed
3. **Test basic perft**: Should get 8902 for perft 3 on starting position
4. **Read this file**: Check current work status and priorities