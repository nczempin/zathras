#!/bin/bash
# Comprehensive test suite for Zathras chess engine
set -e

EXECUTABLE="./zathras"
PASS_COUNT=0
FAIL_COUNT=0

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

log_pass() {
    echo -e "${GREEN}✓ PASS:${NC} $1"
    ((PASS_COUNT++))
}

log_fail() {
    echo -e "${RED}✗ FAIL:${NC} $1"
    ((FAIL_COUNT++))
}

log_info() {
    echo -e "${YELLOW}ℹ INFO:${NC} $1"
}

# Check if executable exists
if [ ! -f "$EXECUTABLE" ]; then
    log_fail "Executable $EXECUTABLE not found. Run 'make' first."
    exit 1
fi

log_info "Starting Zathras Chess Engine Test Suite"
log_info "Testing executable: $EXECUTABLE"

# Test 1: Basic UCI Protocol
log_info "Test 1: Basic UCI Protocol"
if echo -e "uci\nquit" | timeout 10s $EXECUTABLE > /dev/null 2>&1; then
    log_pass "UCI protocol response"
else
    log_fail "UCI protocol response"
fi

# Test 2: Version check
log_info "Test 2: Version Check"
VERSION=$(echo -e "uci\nquit" | timeout 10s $EXECUTABLE 2>/dev/null | head -1 | grep "version")
if [[ $VERSION == *"0.0.5"* ]]; then
    log_pass "Version is 0.0.5"
else
    log_fail "Expected version 0.0.5, got: $VERSION"
fi

# Test 3: Core perft validation (starting position)
log_info "Test 3: Core Perft Validation"
result=$(echo -e "uci\nposition startpos\nperft 3\nquit" | timeout 15s $EXECUTABLE 2>/dev/null | grep "Perft 3 result:" | awk '{print $4}' || echo "TIMEOUT")

if [ "$result" = "8902" ]; then
    log_pass "Perft 3 (startpos): $result nodes"
elif [ "$result" = "TIMEOUT" ]; then
    log_fail "Perft 3 (startpos): Timeout after 15s"
else
    log_fail "Perft 3 (startpos): Expected 8902, got $result"
fi

# Test 4: FEN position support (basic)
log_info "Test 4: FEN Position Support"
# Test with starting position FEN to verify FEN parsing works
startpos_fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
result=$(echo -e "uci\nposition fen $startpos_fen\nperft 1\nquit" | timeout 10s $EXECUTABLE 2>/dev/null | grep "Perft 1 result:" | awk '{print $4}' || echo "TIMEOUT")

if [ "$result" = "20" ]; then
    log_pass "FEN position parsing: $result nodes"
elif [ "$result" = "TIMEOUT" ]; then
    log_fail "FEN position parsing: Timeout after 10s"
else
    log_fail "FEN position parsing: Expected 20, got $result"
fi

# Test 5: Performance check (Perft 3 should be very fast)
log_info "Test 5: Performance Check"
start_time=$(date +%s.%N)
result=$(echo -e "uci\nposition startpos\nperft 3\nquit" | timeout 10s $EXECUTABLE 2>/dev/null | grep "Perft 3 result:" | awk '{print $4}' || echo "TIMEOUT")
end_time=$(date +%s.%N)
duration=$(echo "$end_time - $start_time" | bc -l)

if [ "$result" = "8902" ] && (( $(echo "$duration < 5.0" | bc -l) )); then
    log_pass "Performance: Perft 3 completed in ${duration}s (< 5s)"
elif [ "$result" = "TIMEOUT" ]; then
    log_fail "Performance: Perft 3 timeout"
else
    log_fail "Performance: Perft 3 took ${duration}s (should be < 5s)"
fi

# Test 6: UCI ID command
log_info "Test 6: UCI ID Commands"
uci_output=$(echo -e "uci\nquit" | timeout 10s $EXECUTABLE 2>/dev/null)
if echo "$uci_output" | grep -q "id name" && echo "$uci_output" | grep -q "id author"; then
    log_pass "UCI ID commands present"
else
    log_fail "UCI ID commands missing"
fi

# Test 7: Basic stability test
log_info "Test 7: Basic Stability Test"
if echo -e "uci\nposition startpos\nperft 3\nperft 3\nperft 3\nquit" | timeout 30s $EXECUTABLE > /dev/null 2>&1; then
    log_pass "Multiple perft commands executed successfully"
else
    log_fail "Multiple perft commands failed"
fi

# Summary
echo
log_info "=== TEST SUMMARY ==="
log_info "Total tests: $((PASS_COUNT + FAIL_COUNT))"
log_info "Passed: $PASS_COUNT"
log_info "Failed: $FAIL_COUNT"

if [ $FAIL_COUNT -eq 0 ]; then
    echo -e "${GREEN}🎉 ALL TESTS PASSED!${NC}"
    exit 0
else
    echo -e "${RED}❌ $FAIL_COUNT TEST(S) FAILED!${NC}"
    exit 1
fi