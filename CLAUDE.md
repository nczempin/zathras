# CLAUDE.md - Zathras Chess Engine

Guidance for Claude Code when working on the Zathras chess engine.

Project status, open bugs and priorities live in GitHub issues. The version lives in `zathras_lib/src/zathras_lib.h`. CI steps live in `.github/workflows/ci.yml`. Don't copy any of that into this file.

## Repository Overview

**Zathras** is a UCI chess engine written in C++17, built with Make on Linux. It values correctness and educational value over raw performance.

## Branches

- `dev` is the default branch and the base for all work. `main` holds releases.
- Feature branches are named `feature/ZTH-{issue-number}-{description}` (project ID `ZTH`) and target `dev`.
- Merge `dev` into `main` for a release.

## Build and Test

```bash
make clean && make                                                 # object files go to build/, never src/
echo -e "uci\nquit" | ./zathras                                    # UCI smoke test
echo -e "uci\nposition startpos\nperft 4\nquit" | ./zathras        # perft
echo -e "uci\nposition startpos\ndivide 3\nquit" | ./zathras       # per-move perft, for debugging mismatches
./scripts/run_all_ci_tests.sh                                      # full perft suite used by CI
```

Reference perft values for quick sanity checks:

| Position | Depth | Nodes |
| -------- | ----- | ----- |
| Start position | 3 | 8,902 |
| Start position | 4 | 197,281 |
| `4k3/P7/8/8/8/8/8/4K3 w - - 0 1` (promotion) | 2 | 41 |

## Working Rules

- Validate move-generation or make/unmake changes with perft before calling them done.
- Compare against Stockfish's `go perft` with `divide` when a perft count is off.

## Recovery Checklist

1. Check you are on `dev` or a `feature/ZTH-*` branch.
2. `make clean && make` must succeed.
3. Start-position perft 3 must give 8,902.
