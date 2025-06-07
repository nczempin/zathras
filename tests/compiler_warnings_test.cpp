#include "external/catch_amalgamated.hpp"
#include "Position.h"
#include "Move_generator.h"
#include "Move.h"
#include "Searcher.h"
#include "Square.h"

using namespace positions;
using namespace zathras_lib::moves;

TEST_CASE("Code quality - unused parameters and variables", "[code-quality]") {
    Square::init_squares();
    
    SECTION("Move generator methods with unused parameters") {
        Position pos = Position::create_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        Move_generator mg;
        
        // These methods currently have unused parameter warnings
        INFO("Testing methods that have unused parameter warnings");
        
        // Note: has_captured_piece is private, so we can't test it directly
        // Just document that it has unused parameter warnings
        INFO("has_captured_piece method has unused 'moving' parameter");
    }
    
    SECTION("Searcher methods with unused parameters") {
        // Test that Searcher compiles and works despite warnings
        Searcher searcher;
        Position pos = Position::create_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        
        // This should work despite unused parameter warnings in shouldBeIgnored function
        REQUIRE_NOTHROW([&]() {
            Move_generator mg;
            Move_container moves = mg.generate_pseudolegal_moves(pos, 1);
            if (moves.size() > 0) {
                Move best = searcher.findBestmove(moves.get_moves(), pos);
                // Should find some move
                REQUIRE(get_from(best) < 64);
                REQUIRE(get_to(best) < 64);
            }
        }());
    }
}

TEST_CASE("Code quality - unused functions and variables", "[code-quality]") {
    SECTION("Move.h to_string function") {
        // The to_string function in Move.h is defined but never used
        // This test documents the issue
        INFO("Move.h contains unused to_string function causing warnings");
        
        // We could test the function here to make it "used"
        Move test_move = 0;
        set_from(test_move, E2);
        set_to(test_move, E4);
        REQUIRE(get_from(test_move) == E2);
        REQUIRE(get_to(test_move) == E4);
    }
    
    SECTION("Bitboard unused variables") {
        Square::init_squares();
        
        // Test bitboard functionality despite warnings
        bb test_bb = 0;
        Square::set_bit(test_bb, E4);
        
        std::vector<square_t> visited;
        Bitboard::visit_bitboard(test_bb, [&](square_t sq) {
            visited.push_back(sq);
        });
        
        REQUIRE(visited.size() == 1);
        REQUIRE(visited[0] == E4);
    }
}