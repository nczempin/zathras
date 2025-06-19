# Zathras Chess Engine Roadmap

## Current State (December 2024)

The engine is in a broken state with fundamental move generation issues that must be fixed before any other development.

### Critical Issues
- Engine crashes on basic perft tests (assertion failure: `moving != 0`)
- Tests don't compile due to namespace errors
- Move generation correctness is completely unverified
- No debugging tools available (divide command)

## Phase 1: Move Generator Recovery (Weeks 1-3)

**Goal**: 100% correct move generation validated by perft tests

### Week 1: Emergency Fixes
- [ ] Fix assertion failure in Position::make_move (#54)
- [ ] Fix test compilation errors (#60)
- [ ] Get perft(1) working from starting position
- [ ] Implement divide command (#58)

### Week 2: Validation Infrastructure
- [ ] Create automated perft test suite (#59)
- [ ] Add Chess Programming Wiki test positions
- [ ] Set up comparison with Stockfish perft
- [ ] Document debugging workflow

### Week 3: Fix All Move Generation Bugs
- [ ] Fix all failing perft tests up to depth 4
- [ ] Verify special moves (castling, en passant, promotions)
- [ ] Handle edge cases (pins, checks, discovered attacks)
- [ ] Add regression tests for each bug fixed

## Phase 2: Quality & Infrastructure (Week 4)

**Only after move generation is 100% correct**

- [ ] Set up GitHub Actions CI/CD
- [ ] Add code coverage reporting
- [ ] Fix all compiler warnings
- [ ] Remove dead code
- [ ] Apply consistent code formatting

## Phase 3: Architecture Cleanup (Weeks 5-6)

**Only after tests are passing**

- [ ] Separate UCI interface from engine core
- [ ] Remove unnecessary async complexity
- [ ] Simplify command pattern implementation
- [ ] Modernize C++ code (smart pointers, constexpr, etc.)
- [ ] Improve code organization

## Phase 4: Engine Improvements (Future)

**Only after clean, working codebase**

- [ ] Fix search algorithm issues
- [ ] Improve evaluation function
- [ ] Add time management
- [ ] Consider multithreading (if beneficial)
- [ ] Performance optimizations

## Key Principles

1. **Move generator first** - Nothing else matters if moves are wrong
2. **Correctness before features** - Make it work, then make it better
3. **Tests before changes** - Every bug gets a regression test
4. **Simple before complex** - Remove unnecessary complexity

## Success Metrics

### Phase 1 Complete When:
- All Chess Programming Wiki positions pass perft(4)
- No crashes on any legal position
- Automated tests run on every commit
- Divide command available for debugging

### Long-term Goals:
- Clean, modern C++ codebase
- Comprehensive test coverage
- Active development community
- Competitive playing strength

## References

- [Chess Programming Wiki - Perft Results](https://www.chessprogramming.org/Perft_Results)
- [Stockfish](https://github.com/official-stockfish/Stockfish) - Reference implementation
- [UCI Protocol](https://www.wbec-ridderkerk.nl/html/UCIProtocol.html)