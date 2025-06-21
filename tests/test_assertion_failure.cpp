#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "Position.h"
#include "Square.h"
#include "Move_generator.h"
#include "Move.h"
#include "Move_state.h"

using namespace positions;
using namespace zathras_lib::moves;

// Known positions that often expose bugs
struct TestPosition {
    std::string fen;
    std::string description;
    int depth;
};

std::vector<TestPosition> test_positions = {
    // Start position with deep search
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", "Starting position", 5},
    
    // Position after 1.e4 e5 2.Nf3
    {"rnbqkbnr/pppp1ppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2", "After 1.e4 e5 2.Nf3", 4},
    
    // Complex middle game position
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", "Complex position", 4},
    
    // Position with en passant
    {"rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3", "En passant position", 4},
    
    // Endgame position
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", "Endgame position", 5},
    
    // Position with promotions
    {"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1", "Promotion position", 4},
};

uint64_t perft(Position& pos, int depth) {
    if (depth == 0) return 1;
    
    Move_generator mg;
    Move_container moves = mg.generate_pseudolegal_moves(pos, depth);
    auto move_list = moves.get_moves();
    
    uint64_t nodes = 0;
    std::stack<Move> move_stack;
    
    for (size_t i = 0; i < moves.size(); ++i) {
        Move move = move_list[i];
        Move_state ms;
        
        pos.make_move(move, ms, &move_stack);
        move_stack.push(move);
        
        if (!pos.is_in_check(!pos.white_to_move, &move_stack)) {
            nodes += perft(pos, depth - 1);
        }
        
        move_stack.pop();
        pos.unmake_move(move, ms);
    }
    
    return nodes;
}

void test_position(const TestPosition& pos) {
    std::cout << "\nTesting: " << pos.description << std::endl;
    std::cout << "FEN: " << pos.fen << std::endl;
    std::cout << "Depth: " << pos.depth << std::endl;
    
    try {
        Position p = Position::create_position(pos.fen);
        
        // Try perft at increasing depths
        for (int d = 1; d <= pos.depth; ++d) {
            std::cout << "  Perft(" << d << ")... " << std::flush;
            
            uint64_t result = perft(p, d);
            
            std::cout << "OK (nodes: " << result << ")" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "UNKNOWN EXCEPTION" << std::endl;
    }
}

int main() {
    std::cout << "Searching for assertion failure..." << std::endl;
    Square::init_squares();
    
    for (const auto& pos : test_positions) {
        test_position(pos);
    }
    
    std::cout << "\nAll tests completed." << std::endl;
    return 0;
}