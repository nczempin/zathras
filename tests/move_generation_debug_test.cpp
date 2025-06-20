#include "external/catch_amalgamated.hpp"
#include "Position.h"
#include "Square.h"
#include "Move_generator.h"
#include "Move.h"
#include <iostream>

using namespace positions;
using namespace zathras_lib::moves;

TEST_CASE("Move generation - debug starting position step by step", "[move-debug]") {
    Square::init_squares();
    Position pos = Position::create_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    SECTION("Position initialization validation") {
        REQUIRE(pos.white_to_move == true);
        
        // Check that pieces are in expected positions
        REQUIRE(pos.get_piece_on(E1) != 0);  // White king should be on e1
        REQUIRE(pos.get_piece_on(E8) != 0);  // Black king should be on e8
        REQUIRE(pos.get_piece_on(A1) != 0);  // White rook should be on a1
        REQUIRE(pos.get_piece_on(H1) != 0);  // White rook should be h1
        
        std::cout << "Position board:\n" << pos.print_board() << std::endl;
    }
    
    SECTION("Pseudolegal move generation") {
        Move_generator mg;
        
        // Test pseudolegal moves first (should not call make_move)
        Move_container pseudo_moves = mg.generate_pseudolegal_moves(pos, 1);
        
        std::cout << "Generated " << pseudo_moves.size() << " pseudolegal moves" << std::endl;
        REQUIRE(pseudo_moves.size() > 0);
        
        // Examine first few moves
        auto moves = pseudo_moves.get_moves();
        for (size_t i = 0; i < std::min(size_t(5), pseudo_moves.size()); ++i) {
            const Move& move = moves[i];
            square_t from = get_from(move);
            square_t to = get_to(move);
            int8_t piece = pos.get_piece_on(from);
            
            std::cout << "Move " << i << ": from=" << int(from) << " to=" << int(to) 
                     << " piece=" << int(piece) << std::endl;
                     
            REQUIRE(piece != 0);  // Should never have empty source square
            REQUIRE(from < 64);
            REQUIRE(to < 64);
        }
    }
}

TEST_CASE("Move generation - legal moves issue reproduction", "[move-debug]") {
    Square::init_squares();
    Position pos = Position::create_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Move_generator mg;
    
    SECTION("Legal move generation should not crash") {
        // This currently fails with assertion error
        INFO("Attempting to generate legal moves...");
        
        // Try to catch the exact failure point
        REQUIRE_NOTHROW([&]() {
            Move_container legal_moves = mg.generate_legal_moves(pos, 1);
            REQUIRE(legal_moves.size() == 20);
        }());
    }
}

TEST_CASE("Move generation - individual move validation", "[move-debug]") {
    Square::init_squares();
    Position pos = Position::create_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    SECTION("Manual move creation and validation") {
        // Test creating a simple pawn move manually
        Move pawn_move = 0;
        set_from(pawn_move, E2);
        set_to(pawn_move, E4);
        
        square_t from = get_from(pawn_move);
        square_t to = get_to(pawn_move);
        int8_t piece = pos.get_piece_on(from);
        
        std::cout << "Manual move: from=" << int(from) << " to=" << int(to) 
                 << " piece=" << int(piece) << std::endl;
                 
        REQUIRE(from == E2);
        REQUIRE(to == E4);
        REQUIRE(piece != 0);  // Should have white pawn on e2
        
        // Test if we can make this move
        Move_state ms;
        REQUIRE_NOTHROW([&]() {
            pos.make_move(pawn_move, ms);
            pos.unmake_move(pawn_move, ms);
        }());
    }
}