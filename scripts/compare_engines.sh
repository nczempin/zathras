#!/bin/bash

# Compare Zathras and Stockfish perft results
# Usage: ./compare_engines.sh <fen> <depth>

FEN="${1:-r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1}"
DEPTH="${2:-3}"

echo "=== Comparing Zathras vs Stockfish ==="
echo "Position: $FEN"
echo "Depth: $DEPTH"
echo

# Test Zathras
echo "=== ZATHRAS RESULTS ==="
echo -e "uci\nposition fen $FEN\ndivide $DEPTH\nquit" | ./zathras | tail -n +20 | head -n -1

echo
echo "=== STOCKFISH RESULTS ==="
echo -e "position fen $FEN\ngo perft $DEPTH divide\nquit" | stockfish | head -n -1 | tail -n +2

echo
echo "=== COMPARISON SUMMARY ==="
ZATHRAS_TOTAL=$(echo -e "uci\nposition fen $FEN\nperft $DEPTH\nquit" | ./zathras | grep "^[0-9]" | tail -1)
STOCKFISH_TOTAL=$(echo -e "position fen $FEN\ngo perft $DEPTH\nquit" | stockfish | grep "Nodes searched:" | cut -d: -f2 | tr -d ' ')

echo "Zathras total: $ZATHRAS_TOTAL"
echo "Stockfish total: $STOCKFISH_TOTAL"

if [ "$ZATHRAS_TOTAL" -eq "$STOCKFISH_TOTAL" 2>/dev/null ]; then
    echo "✅ MATCH: Both engines agree"
else
    DIFF=$((ZATHRAS_TOTAL - STOCKFISH_TOTAL))
    echo "❌ MISMATCH: Difference of $DIFF moves"
fi