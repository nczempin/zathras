#!/bin/bash
# Verify move generation with well-known test positions
# Based on Chess Programming Wiki: https://www.chessprogramming.org/Perft_Results

set -e

EXECUTABLE="./zathras"

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

log_pass() {
    echo -e "${GREEN}✓ PASS:${NC} $1"
}

log_fail() {
    echo -e "${RED}✗ FAIL:${NC} $1"
}

log_info() {
    echo -e "${YELLOW}ℹ INFO:${NC} $1"
}

test_position() {
    local name="$1"
    local fen="$2"
    local depth="$3"
    local expected="$4"
    
    log_info "Testing: $name (depth $depth)"
    
    result=$(echo -e "uci\nposition fen $fen\nperft $depth\nquit" | timeout 30s $EXECUTABLE 2>/dev/null | grep "Perft $depth result:" | awk '{print $4}' || echo "TIMEOUT")
    
    if [ "$result" = "$expected" ]; then
        log_pass "$name: $result nodes (expected $expected)"
        return 0
    elif [ "$result" = "TIMEOUT" ]; then
        log_fail "$name: Timeout after 30s"
        return 1
    else
        log_fail "$name: Got $result, expected $expected"
        return 1
    fi
}

echo "=== Chess Programming Wiki Test Positions ==="

# Position 1: Starting position
test_position "Starting Position" "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 3 8902

# Position 2: Kiwipete (Peter Ellis Jones)
test_position "Kiwipete" "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 3 9467

# Position 3: Position 3 from CPW
test_position "Position 3" "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1" 3 2812

# Position 4: Position 4 from CPW  
test_position "Position 4" "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 2 264

# Position 5: Position 4 from CPW (different depth)
test_position "Position 4 (depth 1)" "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1" 1 48

# Position 6: Position 5 from CPW
test_position "Position 5" "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8" 3 62379

# Position 7: Position 6 from CPW
test_position "Position 6" "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10" 3 89890

echo
echo "=== Test Complete ==="