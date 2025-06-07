#include "external/catch_amalgamated.hpp"
#include "Position.h"
#include "Square.h"
#include "Move_generator.h"

using namespace positions;
using namespace zathras_lib::moves;

TEST_CASE("Move generation - starting position", "[moves]") {
    Square::init_squares();
    Position pos = Position::create_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Move_generator mg;
    
    Move_container moves = mg.generate_pseudolegal_moves(pos, 1);
    
    REQUIRE(moves.size() == 20);  // 20 pseudo-legal moves from starting position
}

TEST_CASE("Move generation - legal moves check", "[moves]") {
    Square::init_squares();
    Position pos = Position::create_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Move_generator mg;
    
    Move_container legal_moves = mg.generate_legal_moves(pos, 1);
    
    REQUIRE(legal_moves.size() == 20);  // All moves should be legal from starting position
}

TEST_CASE("Move generation - endgame position", "[moves]") {
    Square::init_squares();
    Position pos = Position::create_position("8/8/8/8/8/8/8/K1k5 w - - 0 1");
    Move_generator mg;
    
    Move_container moves = mg.generate_legal_moves(pos, 1);
    
    REQUIRE(moves.size() == 3);  // King can move to 3 squares
}

TEST_CASE("Move generation - complex position", "[moves]") {
    Square::init_squares();
    Position pos = Position::create_position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    Move_generator mg;
    
    Move_container moves = mg.generate_legal_moves(pos, 1);
    
    REQUIRE(moves.size() > 40);  // This position should have many legal moves
}