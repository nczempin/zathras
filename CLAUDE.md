# Claude AI Development Guidelines

## Branching Strategy

### Preferred Branch Structure
- **main**: Primary production branch (replaces legacy "master")
- **develop**: Development integration branch where feature PRs are merged
- **feature branches**: Individual feature development following pattern `feature/ZA-{issue}_Description`
- **release branches**: Optional semver-based release preparation (develop → release → main)

### Branch Flow
1. Feature development: `feature/ZA-{issue}_Description` → `develop`
2. Integration testing: `develop` branch
3. Release preparation: `develop` → `release/vX.Y.Z` → `main` (optional)
4. Direct release: `develop` → `main`

### Implementation Notes
- Use proper feature branch naming: `feature/ZA-{issue-number}_{description}`
- All feature PRs should target "develop", not "main"
- Only release/hotfix PRs should target "main" directly
- Use semantic versioning for release branches when applicable
- Create one feature branch per issue - don't mix multiple issues

## AI Development Standards
- Always follow the established branching strategy
- Check target branches before rebasing PRs
- Update repository default branches to align with strategy
- Maintain clean, linear history where possible
- Run tests before committing: `make test`
- Run linting/typechecking if available

## Project Status (as of 2025-06-21)

### Completed Work
- ✅ Issue #60: Fixed test compilation errors (namespace issues)
- ✅ Issue #58: Implemented divide command for perft debugging
- ✅ Integrated Catch2 test framework
- ✅ Fixed compiler warnings

### Known Issues
- 🐛 Issue #54: Assertion failure `moving != 0` in Position::make_move during perft
- 🐛 Move generation bugs: perft(3) returns 8868 instead of expected 8902
- 🐛 Test failures in bitboard ffs functions (off-by-one errors between 0-based and 1-based indexing)

### Next Priority Work
1. **Fix Issue #54**: Use divide command to identify which moves cause the assertion failure
2. **Fix ffs indexing**: Reconcile 0-based vs 1-based indexing in bitboard operations
3. **Fix move generation**: Use Chess Programming Wiki test positions to fix all perft mismatches

### Development Tools Available
- **divide command**: Use `divide <depth>` to debug move generation
- **perft command**: Use `perft <depth>` for performance testing
- **Catch2 tests**: Run with `make test`

### Important Commands
```bash
# Build and test
make clean && make
make test

# Debug move generation
./zathras
position startpos
divide 3
perft 5

# Create feature branch
git checkout -b feature/ZA-{issue}_Description

# Before creating PR
make clean && make && make test
```

### Technical Debt
- Bitboard ffs() function has inconsistent indexing (0-based vs 1-based)
- Move generation creates invalid moves from empty squares
- Test coverage needs improvement
- Some code uses magic numbers instead of constants