#!/bin/bash
# Run all CI tests locally

set -e

echo "=== Running All CI Tests Locally ==="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

failed_tests=0
total_tests=0

# Function to run a test and check result
run_test() {
    local test_name="$1"
    local expected="$2"
    local command="$3"
    
    total_tests=$((total_tests + 1))
    echo -n "Testing $test_name (expected: $expected)... "
    
    result=$(eval "$command")
    
    if [ "$result" = "$expected" ]; then
        echo -e "${GREEN}✅ PASS${NC} ($result)"
    else
        echo -e "${RED}❌ FAIL${NC} (got $result, expected $expected)"
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
run_test "Starting position perft 2" "400" \
    "echo -e 'uci\nposition startpos\nperft 2\nquit' | ./zathras 2>/dev/null | grep 'Perft 2 result:' | awk '{print \$4}'"

run_test "Starting position perft 3" "8902" \
    "echo -e 'uci\nposition startpos\nperft 3\nquit' | ./zathras 2>/dev/null | grep 'Perft 3 result:' | awk '{print \$4}'"

run_test "Starting position perft 4" "197281" \
    "echo -e 'uci\nposition startpos\nperft 4\nquit' | ./zathras 2>/dev/null | grep 'Perft 4 result:' | awk '{print \$4}'"

# Kiwipete position
run_test "Kiwipete position perft 3" "97862" \
    "echo -e 'uci\nposition fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -\nperft 3\nquit' | ./zathras 2>/dev/null | grep 'Perft 3 result:' | awk '{print \$4}'"

# Position 4 tests
run_test "Position 4 perft 3" "9467" \
    "echo -e 'uci\nposition fen r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1\nperft 3\nquit' | ./zathras 2>/dev/null | grep 'Perft 3 result:' | awk '{print \$4}'"

run_test "Position 4 perft 4" "422333" \
    "echo -e 'uci\nposition fen r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1\nperft 4\nquit' | ./zathras 2>/dev/null | grep 'Perft 4 result:' | awk '{print \$4}'"

# Position 5 test
run_test "Position 5 perft 3" "62379" \
    "echo -e 'uci\nposition fen rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8\nperft 3\nquit' | ./zathras 2>/dev/null | grep 'Perft 3 result:' | awk '{print \$4}'"

echo ""
echo "=== Promotion Tests ==="

# Simple promotion tests
run_test "White pawn promotion perft 2" "41" \
    "echo -e 'uci\nposition fen 4k3/P7/8/8/8/8/8/4K3 w - - 0 1\nperft 2\nquit' | ./zathras 2>/dev/null | grep 'Perft 2 result:' | awk '{print \$4}'"

run_test "Black pawn promotion perft 2" "41" \
    "echo -e 'uci\nposition fen 4k3/8/8/8/8/8/p7/4K3 b - - 0 1\nperft 2\nquit' | ./zathras 2>/dev/null | grep 'Perft 2 result:' | awk '{print \$4}'"

echo ""
echo "=== Edge File Promotion Tests ==="

# Edge file promotions
run_test "White pawn on a7 perft 1" "9" \
    "echo -e 'uci\nposition fen 4k3/P7/8/8/8/8/8/4K3 w - - 0 1\nperft 1\nquit' | ./zathras 2>/dev/null | grep 'Perft 1 result:' | awk '{print \$4}'"

run_test "Black pawn on a2 perft 1" "9" \
    "echo -e 'uci\nposition fen 4k3/8/8/8/8/8/p7/4K3 b - - 0 1\nperft 1\nquit' | ./zathras 2>/dev/null | grep 'Perft 1 result:' | awk '{print \$4}'"

run_test "White pawn on h7 perft 1" "9" \
    "echo -e 'uci\nposition fen 4k3/7P/8/8/8/8/8/4K3 w - - 0 1\nperft 1\nquit' | ./zathras 2>/dev/null | grep 'Perft 1 result:' | awk '{print \$4}'"

run_test "Black pawn on h2 perft 1" "9" \
    "echo -e 'uci\nposition fen 4k3/8/8/8/8/8/7p/4K3 b - - 0 1\nperft 1\nquit' | ./zathras 2>/dev/null | grep 'Perft 1 result:' | awk '{print \$4}'"

echo ""
echo "=== Divide Command Tests ==="

# Test divide command
echo -n "Testing divide command output format... "
divide_output=$(echo -e "uci\nposition startpos\ndivide 2\nquit" | timeout 30s ./zathras 2>/dev/null)
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
promo_count=$(echo -e "uci\nposition fen 4k3/P7/8/8/8/8/8/4K3 w - - 0 1\ndivide 1\nquit" | ./zathras 2>/dev/null | grep -E "^a7a8[qrbn]:" | wc -l)
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