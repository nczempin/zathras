#!/bin/bash
# Run linting tools with graceful handling of missing tools

echo "=== C++ Linting Report for Zathras ==="
echo ""

# Track if any linter was run
LINTER_RUN=false

# Run clang-tidy if available
if command -v clang-tidy &> /dev/null; then
    echo "Running clang-tidy..."
    find src zathras_lib/src -name '*.cpp' -o -name '*.h' | \
        xargs clang-tidy --config-file=.clang-tidy 2>/dev/null || true
    LINTER_RUN=true
    echo ""
else
    echo "⚠️  clang-tidy not found. Skipping clang-tidy checks."
    echo "   To install: sudo apt-get install clang-tidy"
    echo ""
fi

# Run cppcheck if available
if command -v cppcheck &> /dev/null; then
    echo "Running cppcheck..."
    cppcheck --enable=all --std=c++17 --inline-suppr \
        --suppress=missingIncludeSystem \
        -I./zathras_lib/src -I./src \
        src/*.cpp zathras_lib/src/*.cpp \
        --error-exitcode=0 2>&1 || true
    LINTER_RUN=true
else
    echo "⚠️  cppcheck not found. Skipping cppcheck checks."
    echo "   To install: sudo apt-get install cppcheck"
    echo ""
fi

if [ "$LINTER_RUN" = false ]; then
    echo "❌ No linting tools found. Please install at least one of:"
    echo "   - clang-tidy"
    echo "   - cppcheck"
    exit 1
fi

echo ""
echo "=== Linting complete ==="