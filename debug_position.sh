#!/bin/bash
# Debug specific position after a move

FEN="r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
MOVE="b4c5"

echo "=== Position after $MOVE ==="
echo ""

# Show position after move in both engines
echo "=== Stockfish evaluation ==="
echo -e "position fen $FEN moves $MOVE\nd" | stockfish | grep -A 20 "Fen:"

echo ""
echo "=== Zathras position ==="
echo -e "uci\nposition fen $FEN moves $MOVE\nperft 0\nquit" | ./zathras | grep -A 30 "wtm:"

echo ""
echo "=== Move count comparison ==="
echo "Stockfish move count:"
echo -e "position fen $FEN moves $MOVE\ngo perft 1" | stockfish | grep "Nodes searched" | awk '{print $3}'

echo ""
echo "Zathras move count:"
echo -e "uci\nposition fen $FEN moves $MOVE\nperft 1\nquit" | ./zathras | grep "Perft 1 result:" | awk '{print $4}'

echo ""
echo "=== Detailed move lists ==="
echo "Stockfish moves:"
echo -e "position fen $FEN moves $MOVE\ngo perft 1" | stockfish | grep -E "^[a-h][1-8]" | sort

echo ""
echo "Zathras moves:"
echo -e "uci\nposition fen $FEN moves $MOVE\ndivide 1\nquit" | ./zathras | grep -E "^[a-h][1-8]" | awk '{print $1}' | sort