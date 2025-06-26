#!/bin/bash

echo "=== Testing Issue #109: Black Queenside Castling Bug ==="
echo "This test verifies the fix for the black queenside castling bug where"
echo "the rook was incorrectly placed on c8 instead of d8."
echo ""

echo "Test 1: Show position after e8c8"
echo "================================="
echo -e "uci\nposition fen r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R b KQkq - 0 1 moves e8c8\nd\nquit" | ./zathras 2>/dev/null | grep -A8 "8 |" | head -1
echo "Expected: 8 | . . k r . . . r |"
echo ""

echo "Test 2: Verify correct king moves from c8"
echo "========================================="
echo "After the fix, the king on c8 should only make normal king moves:"
echo -e "uci\nposition fen r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R b KQkq - 0 1 moves e8c8\ndivide 1\nquit" | ./zathras 2>/dev/null | grep "^c8" | sort
echo ""

echo "Test 3: Verify checkmate detection is now correct"
echo "================================================="
echo "After c4c5 e8c8 a7a8q, the position is checkmate (0 moves):"
echo ""
echo -e "uci\nposition fen r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 moves c4c5 e8c8 a7a8q\ndivide 1\nquit" | ./zathras 2>/dev/null | grep -E "(^c8|Nodes)"
echo ""
echo "Test 4: Verify Position 4 perft 4 is now correct"
echo "==============================================="
echo -e "position fen r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1\nperft 4\nquit" | ./zathras 2>/dev/null | grep "Perft 4 result:"
echo "Expected: 422333"