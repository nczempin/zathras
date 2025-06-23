#!/bin/bash
# Check for case sensitivity issues in include statements

set -e

echo "Checking for case sensitivity issues..."

# Get all header files
header_files=$(find src/ zathras_lib/src/ -name "*.h" -type f 2>/dev/null | sed 's|.*/||' | sort -u)

# Check all includes
errors=0
while IFS= read -r line; do
    if [[ $line =~ \#include[[:space:]]*[\"\<]([^\"\>/]+\.h)[\"\>] ]]; then
        included_file="${BASH_REMATCH[1]}"
        # Check if this is a local header (not system header)
        if ! [[ $included_file =~ ^(iostream|string|vector|array|memory|chrono|future|thread|cassert|ctime|bitset|utility|algorithm|cstdlib|set|deque|climits|sstream|iomanip|cstdint|functional|stddef|intrin)\.h$ ]]; then
            # Check if file exists with exact case
            found=0
            for header in $header_files; do
                if [[ "$header" == "$included_file" ]]; then
                    found=1
                    break
                fi
            done
            if [[ $found -eq 0 ]]; then
                # Check if it exists with different case
                for header in $header_files; do
                    if [[ "${header,,}" == "${included_file,,}" ]]; then
                        echo "ERROR: Case mismatch in include: '$included_file' should be '$header'"
                        echo "  Found in: $line"
                        errors=$((errors + 1))
                        break
                    fi
                done
            fi
        fi
    fi
done < <(grep -r "#include" src/ zathras_lib/src/ --include="*.cpp" --include="*.h" 2>/dev/null || true)

# Check for duplicate files with different cases
echo "Checking for duplicate files with different cases..."
duplicate_files=$(git ls-files | sort -f | uniq -di)
if [[ -n "$duplicate_files" ]]; then
    echo "ERROR: Found duplicate files with different cases:"
    echo "$duplicate_files"
    errors=$((errors + 1))
fi

# Check for pragma once in .cpp files
echo "Checking for misplaced #pragma once..."
pragma_errors=$(grep -l "^#pragma once" src/*.cpp zathras_lib/src/*.cpp 2>/dev/null || true)
if [[ -n "$pragma_errors" ]]; then
    echo "ERROR: Found #pragma once in .cpp files:"
    echo "$pragma_errors"
    errors=$((errors + 1))
fi

if [[ $errors -eq 0 ]]; then
    echo "✓ No case sensitivity issues found"
    exit 0
else
    echo "✗ Found $errors case sensitivity issues"
    exit 1
fi