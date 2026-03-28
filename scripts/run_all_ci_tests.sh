#!/usr/bin/env bash
# Run all CI tests locally

echo "=== Running All CI Tests Locally ==="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

failed_tests=0
total_tests=0

# Function to run a perft test - accepts position_str (e.g. "startpos" or "fen <FEN>"), depth, expected
run_perft_test() {
    local test_name="$1"
    local position_str="$2"
    local depth="$3"
    local expected="$4"
    local engine_output
    local result

    total_tests=$((total_tests + 1))
    echo -n "Testing $test_name (expected: $expected)... "

    engine_output=$(echo -e "uci\nposition $position_str\nperft $depth\nquit" | timeout 10s ./zathras 2>&1)
    result=$(echo "$engine_output" | grep "Perft $depth result:" | awk '{print $4}')

    if [ "$result" = "$expected" ]; then
        echo -e "${GREEN}✅ PASS${NC} ($result)"
    else
        echo -e "${RED}❌ FAIL${NC} (got ${result:-"<empty>"}, expected $expected)"
        echo -e "${YELLOW}--- Engine output ---${NC}"
        echo "$engine_output"
        echo -e "${YELLOW}--- End of engine output ---${NC}"
        failed_tests=$((failed_tests + 1))
    fi
}

# Test 1: Basic UCI functionality
echo "=== Basic Functionality Tests ==="
echo -n "Testing basic UCI protocol... "
if echo -e "uci\nquit" | timeout 10s ./zathras > /dev/null 2>&1; then
    echo -e "${GREEN}✅ PASS${NC}"
else
    echo -e "${RED}❌ FAIL${NC}"
    failed_tests=$((failed_tests + 1))
fi
total_tests=$((total_tests + 1))

echo ""
echo "=== Perft Tests ==="

# Starting position tests
run_perft_test "Starting position perft 2" "startpos" 2 "400"

run_perft_test "Starting position perft 3" "startpos" 3 "8902"

run_perft_test "Starting position perft 4" "startpos" 4 "197281"

# Kiwipete position
run_perft_test "Kiwipete position perft 3" "fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -" 3 "97862"

# Position 4 tests
run_perft_test "Position 4 perft 3" "fen r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 3 "9467"

run_perft_test "Position 4 perft 4" "fen r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 4 "422333"

# Position 5 test
run_perft_test "Position 5 perft 3" "fen rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" 3 "62379"

echo ""
echo "=== Promotion Tests ==="

# Simple promotion tests
run_perft_test "White pawn promotion perft 2" "fen 4k3/P7/8/8/8/8/8/4K3 w - - 0 1" 2 "41"

run_perft_test "Black pawn promotion perft 2" "fen 4k3/8/8/8/8/8/p7/4K3 b - - 0 1" 2 "41"

echo ""
echo "=== Edge File Promotion Tests ==="

# Edge file promotions
run_perft_test "White pawn on a7 perft 1" "fen 4k3/P7/8/8/8/8/8/4K3 w - - 0 1" 1 "9"

run_perft_test "Black pawn on a2 perft 1" "fen 4k3/8/8/8/8/8/p7/4K3 b - - 0 1" 1 "9"

run_perft_test "White pawn on h7 perft 1" "fen 4k3/7P/8/8/8/8/8/4K3 w - - 0 1" 1 "9"

run_perft_test "Black pawn on h2 perft 1" "fen 4k3/8/8/8/8/8/7p/4K3 b - - 0 1" 1 "9"

echo ""
echo "=== Divide Command Tests ==="

# Test divide command
echo -n "Testing divide command output format... "
divide_output=$(echo -e "uci\nposition startpos\ndivide 2\nquit" | timeout 30s ./zathras 2>&1)
if echo "$divide_output" | grep -q "Divide .* result:" && \
   [ "$(echo "$divide_output" | grep -E "^[a-h][1-8][a-h][1-8].*:" | wc -l)" -eq 20 ] && \
   [ "$(echo "$divide_output" | grep "Nodes searched:" | awk '{print $3}')" = "400" ]; then
    echo -e "${GREEN}✅ PASS${NC}"
else
    echo -e "${RED}❌ FAIL${NC}"
    failed_tests=$((failed_tests + 1))
fi
total_tests=$((total_tests + 1))

# Test divide with promotions
echo -n "Testing divide command with promotions... "
promo_count=$(echo -e "uci\nposition fen 4k3/P7/8/8/8/8/8/4K3 w - - 0 1\ndivide 1\nquit" | timeout 30s ./zathras 2>&1 | grep -E "^a7a8[qrbn]:" | wc -l)
if [ "$promo_count" -eq 4 ]; then
    echo -e "${GREEN}✅ PASS${NC}"
else
    echo -e "${RED}❌ FAIL${NC} (found $promo_count promotion moves, expected 4)"
    failed_tests=$((failed_tests + 1))
fi
total_tests=$((total_tests + 1))

echo ""
echo "=== TEST SUMMARY ==="
echo "Total tests: $total_tests"
echo "Passed: $((total_tests - failed_tests))"
echo "Failed: $failed_tests"

if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}✅ All CI tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed!${NC}"
    exit 1
fi