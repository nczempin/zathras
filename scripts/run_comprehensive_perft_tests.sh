#!/bin/bash
# Comprehensive perft test suite based on Chess Programming Wiki positions
# https://www.chessprogramming.org/Perft_Results

set -e

echo "=== Comprehensive Perft Test Suite ==="
echo "Testing all standard perft positions from Chess Programming Wiki"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

failed_tests=0
total_tests=0
start_time=$(date +%s)

# Function to run a perft test
run_perft_test() {
    local position_name="$1"
    local fen="$2"
    local depth="$3"
    local expected="$4"
    
    total_tests=$((total_tests + 1))
    echo -n "Testing $position_name perft $depth (expected: $expected)... "
    
    result=$(echo -e "uci\nposition fen $fen\nperft $depth\nquit" | timeout 300s ./zathras 2>/dev/null | grep "Perft $depth result:" | awk '{print $4}')
    
    if [ "$result" = "$expected" ]; then
        echo -e "${GREEN}✅ PASS${NC} ($result)"
        return 0
    else
        echo -e "${RED}❌ FAIL${NC} (got $result, expected $expected)"
        failed_tests=$((failed_tests + 1))
        return 1
    fi
}

echo "=== Position 1: Initial Position ==="
echo "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
run_perft_test "Initial position" "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 1 20
run_perft_test "Initial position" "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 2 400
run_perft_test "Initial position" "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 3 8902
run_perft_test "Initial position" "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 4 197281

echo ""
echo "=== Position 2: Kiwipete ==="
echo "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -"
run_perft_test "Kiwipete" "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -" 1 48
run_perft_test "Kiwipete" "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -" 2 2039
run_perft_test "Kiwipete" "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -" 3 97862
run_perft_test "Kiwipete" "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -" 4 4085603

echo ""
echo "=== Position 3 ==="
echo "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -"
run_perft_test "Position 3" "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -" 1 14
run_perft_test "Position 3" "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -" 2 191
run_perft_test "Position 3" "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -" 3 2812
run_perft_test "Position 3" "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -" 4 43238

echo ""
echo "=== Position 4 ==="
echo "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
run_perft_test "Position 4" "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 1 6
run_perft_test "Position 4" "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 2 264
run_perft_test "Position 4" "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 3 9467
run_perft_test "Position 4" "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 4 422333

echo ""
echo "=== Position 5 ==="
echo "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"
run_perft_test "Position 5" "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" 1 44
run_perft_test "Position 5" "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" 2 1486
run_perft_test "Position 5" "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" 3 62379
run_perft_test "Position 5" "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" 4 2103487

echo ""
echo "=== Position 6 ==="
echo "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"
run_perft_test "Position 6" "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10" 1 46
run_perft_test "Position 6" "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10" 2 2079
run_perft_test "Position 6" "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10" 3 89890
run_perft_test "Position 6" "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10" 4 3894594

end_time=$(date +%s)
duration=$((end_time - start_time))

echo ""
echo "=== TEST SUMMARY ==="
echo "Total tests: $total_tests"
echo "Passed: $((total_tests - failed_tests))"
echo "Failed: $failed_tests"
echo "Duration: ${duration}s"

if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}✅ All comprehensive perft tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed!${NC}"
    exit 1
fi