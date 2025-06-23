#!/bin/bash
# Git bisect test script for move generation
# Returns 0 (good) if perft(3) = 8902, 1 (bad) otherwise

# set -e  # Don't exit on errors, handle them manually
echo "Testing commit: $(git rev-parse --short HEAD)"

# Create simple test file
cat > simple_perft_test.cpp << 'EOF'
#include <iostream>
#include <climits>
#include "zathras_lib/src/Position.h"
#include "zathras_lib/src/Move_generator.h"

using namespace std;
using namespace Positions;
using namespace Moves;

uint64_t perft(Position& pos, Move_generator& mg, int depth) {
    if (depth == 0) return 1;
    
    Move_container moves = mg.generate_legal_moves(pos, depth);
    if (depth == 1) return moves.size();
    
    uint64_t nodes = 0;
    auto move_list = moves.get_moves();
    
    for (int i = 0; i < moves.size(); i++) {
        Move move = move_list[i];
        Move_state ms;
        pos.make_move(move, ms);
        nodes += perft(pos, mg, depth - 1);
        pos.unmake_move(move, ms);
    }
    
    return nodes;
}

int main() {
    Position pos = Position::create_start_position();
    Move_generator mg;
    
    uint64_t result = perft(pos, mg, 3);
    cout << "Perft 3 result: " << result << endl;
    
    return result == 8902 ? 0 : 1;
}
EOF

# Try to build and run the test
echo "Building simple test..."
# Try to compile only what we need, using files that exist
if ! g++ -o simple_perft_test -std=c++17 simple_perft_test.cpp \
    zathras_lib/src/Position.cpp \
    zathras_lib/src/Move_generator.cpp \
    zathras_lib/src/Move.cpp \
    zathras_lib/src/Square.cpp \
    zathras_lib/src/Piece.cpp \
    zathras_lib/src/Bitboard.cpp \
    zathras_lib/src/Move_container.cpp \
    zathras_lib/src/Move_state.cpp \
    zathras_lib/src/misc.cpp \
    -I. -w 2>&1; then
    echo "Build failed - marking as bad"
    exit 1
fi

# Run the test
echo "Running perft test..."
if ./simple_perft_test; then
    echo "GOOD: Correct perft result (8902)"
    exit 0
else
    echo "BAD: Incorrect perft result or runtime error"
    exit 1
fi