# Scripts Directory

This directory contains utility scripts for development, testing, and debugging of the Zathras chess engine.

## Development Scripts

- **setup_dev_env.sh** - Sets up the development environment
- **run_linting.sh** - Runs code linting and style checks
- **check_case_sensitivity.sh** - Checks for case sensitivity issues in the codebase

## Testing Scripts

- **test_comprehensive_perft.sh** - Runs comprehensive perft validation tests across multiple positions
- **test_suite.sh** - Runs the main test suite for the engine
- **test_verify_positions.sh** - Verifies engine behavior against known positions

## Debugging Scripts

- **debug_bisect_test.sh** - Git bisect helper for finding regression points
- **debug_compare_divide.sh** - Compares move generation between Zathras and Stockfish using divide command
- **debug_position.sh** - Debug utility for analyzing specific chess positions
- **compare_engines.sh** - Compares engine behavior between different versions
- **run_stockfish.sh** - Utility for running Stockfish commands

## Usage

Make sure scripts are executable before running:
```bash
chmod +x scripts/*.sh
```

Most scripts can be run from the project root:
```bash
./scripts/test_comprehensive_perft.sh
./scripts/debug_compare_divide.sh "fen_position" depth
```

Refer to individual script files for specific usage instructions and parameters.