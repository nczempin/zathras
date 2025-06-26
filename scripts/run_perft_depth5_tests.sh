#!/bin/bash
# Perft depth 5 tests - WARNING: These take a long time!
# Only run selected positions that complete in reasonable time

set -e

echo "=== Perft Depth 5 Test Suite ==="
echo "WARNING: These tests may take several minutes each!"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

failed_tests=0
total_tests=0
start_time=$(date +%s)

# Function to run a perft test with timing
run_perft_test() {
    local position_name="$1"
    local fen="$2"
    local depth="$3"
    local expected="$4"
    
    total_tests=$((total_tests + 1))
    echo -n "Testing $position_name perft $depth (expected: $expected)... "
    
    test_start=$(date +%s)
    result=$(echo -e "uci\nposition fen $fen\nperft $depth\nquit" | timeout 600s ./zathras 2>/dev/null | grep "Perft $depth result:" | awk '{print $4}')
    test_end=$(date +%s)
    test_duration=$((test_end - test_start))
    
    if [ "$result" = "$expected" ]; then
        echo -e "${GREEN}✅ PASS${NC} ($result) - ${test_duration}s"
        return 0
    else
        echo -e "${RED}❌ FAIL${NC} (got $result, expected $expected) - ${test_duration}s"
        failed_tests=$((failed_tests + 1))
        return 1
    fi
}

echo "=== Selected Perft Depth 5 Tests ==="
echo "(Only testing positions that complete in reasonable time)"
echo ""

# Initial position - depth 5 is manageable
echo "Position 1: Initial Position"
run_perft_test "Initial position" "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 5 4865609

echo ""
echo "Position 3: Endgame position (faster)"
run_perft_test "Position 3" "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -" 5 674624

echo ""
echo "=== Additional Fast Positions for Depth 5 ==="

# Simple king and pawns endgame
echo "Simple K+P endgame"
run_perft_test "K+P endgame" "8/8/8/8/3k4/8/3P4/3K4 w - - 0 1" 5 34822

# Rook endgame
echo "Rook endgame"
run_perft_test "Rook endgame" "8/8/8/8/8/8/R7/k1K5 w - - 0 1" 5 25332

end_time=$(date +%s)
duration=$((end_time - start_time))

echo ""
echo "=== DEPTH 5 TEST SUMMARY ==="
echo "Total tests: $total_tests"
echo "Passed: $((total_tests - failed_tests))"
echo "Failed: $failed_tests"
echo "Total duration: ${duration}s"

if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}✅ All perft depth 5 tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed!${NC}"
    exit 1
fi