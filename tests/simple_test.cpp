#include <iostream>
#include <cassert>
#include "Position.h"
#include "Square.h"
#include "Move_generator.h"

using namespace positions;
using namespace zathras_lib::moves;

int main() {
    std::cout << "Running simple tests..." << std::endl;
    
    // Initialize squares
    Square::init_squares();
    
    // Test FEN parsing
    Position pos = Position::create_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    assert(pos.white_to_move == true);
    std::cout << "✓ FEN parsing test passed" << std::endl;
    
    // Test move generation
    Move_generator mg;
    Move_container moves = mg.generate_legal_moves(pos, 1);
    assert(moves.size() == 20);
    std::cout << "✓ Move generation test passed (found " << moves.size() << " moves)" << std::endl;
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}