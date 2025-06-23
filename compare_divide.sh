#!/bin/bash
# Compare divide results between Zathras and Stockfish
# Usage: ./compare_divide.sh "FEN" depth

set -e

if [ $# -lt 2 ]; then
    echo "Usage: $0 \"FEN\" depth"
    echo "Example: $0 \"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\" 3"
    exit 1
fi

FEN="$1"
DEPTH="$2"

echo "=== Comparing divide $DEPTH results ==="
echo "FEN: $FEN"
echo ""

# First rebuild Zathras
echo "Building Zathras..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1

# Get Stockfish results
echo "=== Stockfish divide $DEPTH ==="
echo -e "position fen $FEN\ngo perft $DEPTH" | stockfish | grep -A 100 "^[a-h][1-8][a-h][1-8]" | grep -B 100 "Nodes searched" | head -n -1 | sort > stockfish_divide.txt
cat stockfish_divide.txt

# Get total from Stockfish
STOCKFISH_TOTAL=$(echo -e "position fen $FEN\ngo perft $DEPTH" | stockfish | grep "Nodes searched" | awk '{print $3}')
echo ""
echo "Stockfish total: $STOCKFISH_TOTAL"

# Get Zathras results
echo ""
echo "=== Zathras divide $DEPTH ==="
echo -e "uci\nposition fen $FEN\ndivide $DEPTH\nquit" | ./zathras | grep -E "^[a-h][1-8][a-h][1-8]" | awk '{print $1 ": " $2}' | sort > zathras_divide.txt
cat zathras_divide.txt

# Get total from Zathras
ZATHRAS_TOTAL=$(echo -e "uci\nposition fen $FEN\ndivide $DEPTH\nquit" | ./zathras | grep "Total:" | awk '{print $2}')
echo ""
echo "Zathras total: $ZATHRAS_TOTAL"

# Compare results
echo ""
echo "=== Differences ==="
diff -u stockfish_divide.txt zathras_divide.txt || true

# Summary
echo ""
echo "=== Summary ==="
if [ "$STOCKFISH_TOTAL" = "$ZATHRAS_TOTAL" ]; then
    echo "✅ Totals match: $STOCKFISH_TOTAL"
else
    echo "❌ Totals differ: Stockfish=$STOCKFISH_TOTAL, Zathras=$ZATHRAS_TOTAL"
    echo "   Difference: $((STOCKFISH_TOTAL - ZATHRAS_TOTAL)) nodes"
fi

# Cleanup
rm -f stockfish_divide.txt zathras_divide.txt