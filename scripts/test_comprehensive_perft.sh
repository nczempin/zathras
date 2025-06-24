#!/bin/bash
# Comprehensive Perft Test Suite for Zathras Chess Engine
# Tests move generation correctness across various positions and move types

set -e

echo "=== Comprehensive Move Generator Perft Validation ==="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to run a single perft test
run_perft_test() {
    local description="$1"
    local fen="$2"
    local depth="$3"
    local expected="$4"
    
    echo ""
    echo "=== Testing: $description ==="
    echo "FEN: $fen"
    echo "Depth: $depth, Expected: $expected"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Run the test and capture output
    echo "uci" > temp_test.txt
    echo "position fen $fen" >> temp_test.txt
    echo "perft $depth" >> temp_test.txt
    echo "quit" >> temp_test.txt
    
    output=$(timeout 30s ./zathras < temp_test.txt 2>&1)
    rm -f temp_test.txt
    
    # Extract the result
    result=$(echo "$output" | grep "Perft $depth result:" | awk '{print $4}')
    
    if [ -z "$result" ]; then
        echo -e "${RED}FAILED: No result found in output${NC}"
        echo "Output was:"
        echo "$output"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
    
    if [ "$result" = "$expected" ]; then
        echo -e "${GREEN}PASSED: $result${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        return 0
    else
        echo -e "${RED}FAILED: Expected $expected, got $result${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
}

# Function to test a position with multiple depths
test_position() {
    local description="$1"
    local fen="$2"
    shift 2
    
    echo ""
    echo "==========================================="
    echo "Position: $description"
    echo "FEN: $fen"
    echo "==========================================="
    
    # Arguments come in pairs: depth expected depth expected ...
    while [ $# -ge 2 ]; do
        local depth="$1"
        local expected="$2"
        shift 2
        
        run_perft_test "$description (depth $depth)" "$fen" "$depth" "$expected"
    done
}

# Build the engine first
echo "Building Zathras..."
make clean > /dev/null 2>&1
if ! make > /dev/null 2>&1; then
    echo -e "${RED}FAILED: Could not build Zathras${NC}"
    exit 1
fi
echo -e "${GREEN}Build successful${NC}"

echo ""
echo "Starting comprehensive perft tests..."

# Test 1: Starting position (baseline)
test_position "Starting position" \
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" \
    1 20 \
    2 400 \
    3 8902 \
    4 197281

# Test 2: Kiwipete (comprehensive test of most move types)
test_position "Kiwipete - exercises most move types" \
    "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" \
    1 6 \
    2 264 \
    3 9467 \
    4 422333

# Test 2b: Position 4 from Chess Programming Wiki (promotion intensive)
test_position "Position 4 - intensive promotion testing" \
    "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1" \
    1 6 \
    2 264 \
    3 9467

# Test 3: En passant position
test_position "En passant capture available" \
    "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1" \
    1 20 \
    2 400 \
    3 8902

# Test 4: Complex en passant
test_position "Complex en passant position" \
    "rnbqkb1r/pp1p1ppp/5n2/2pPp3/8/8/PPP1PPPP/RNBQKBNR w KQkq c6 0 3" \
    1 31 \
    2 570

# Test 5: Castling test - both sides available
test_position "Castling availability test" \
    "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1" \
    1 26 \
    2 568 \
    3 13744

# Test 6: No castling rights
test_position "No castling rights" \
    "r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1" \
    1 24 \
    2 496 \
    3 9483

# Test 7: Promotion test
test_position "Pawn promotion test" \
    "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1" \
    1 24 \
    2 496

# Test 8: Under-promotion possibilities
test_position "Under-promotion possibilities" \
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1" \
    1 14 \
    2 191 \
    3 2812

# Test 9: Check evasion
test_position "Check evasion test" \
    "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" \
    1 44 \
    2 1486

# Test 10: Isolated piece movement tests
test_position "Knight mobility test" \
    "8/8/8/4N3/8/8/8/8 w - - 0 1" \
    1 8

test_position "Bishop mobility test" \
    "8/8/8/8/3B4/8/8/8 w - - 0 1" \
    1 13

test_position "Rook mobility test" \
    "8/8/8/8/3R4/8/8/8 w - - 0 1" \
    1 14

test_position "Queen mobility test" \
    "8/8/8/8/3Q4/8/8/8 w - - 0 1" \
    1 27

test_position "King mobility test" \
    "8/8/8/8/3K4/8/8/8 w - - 0 1" \
    1 8

# Test 11: Edge case positions
test_position "Near-stalemate position" \
    "8/8/8/8/8/6k1/6p1/6K1 w - - 0 1" \
    1 3 \
    2 7

echo ""
echo "==========================================="
echo "=== FINAL RESULTS ==="
echo "==========================================="
echo "Total tests run: $TOTAL_TESTS"
echo -e "Passed: ${GREEN}$PASSED_TESTS${NC}"
echo -e "Failed: ${RED}$FAILED_TESTS${NC}"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "\n${GREEN}✅ All tests passed! Move generator appears correct.${NC}"
    exit 0
else
    echo -e "\n${RED}❌ $FAILED_TESTS tests failed. Move generator needs fixes.${NC}"
    echo ""
    echo "Common issues to check:"
    echo "- En passant capture detection"
    echo "- Castling rights and availability"
    echo "- Pawn promotion (including under-promotion)"
    echo "- Check detection and evasion"
    echo "- Legal vs pseudolegal move generation"
    echo "- Bitboard indexing (0-based vs 1-based)"
    exit 1
fi