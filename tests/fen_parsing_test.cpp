#include "external/catch_amalgamated.hpp"
#include "Position.h"
#include "Square.h"

using namespace positions;

TEST_CASE("FEN parsing - starting position", "[fen]") {
    Square::init_squares();
    Position pos = Position::create_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    REQUIRE(pos.white_to_move == true);
    REQUIRE(pos.castling[0] == true);  // White kingside
    REQUIRE(pos.castling[1] == true);  // White queenside  
    REQUIRE(pos.castling[2] == true);  // Black kingside
    REQUIRE(pos.castling[3] == true);  // Black queenside
    REQUIRE(pos.en_passant_square == 0);
}

TEST_CASE("FEN parsing - no castling rights", "[fen]") {
    Square::init_squares();
    Position pos = Position::create_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1");
    
    REQUIRE(pos.white_to_move == true);
    REQUIRE(pos.castling[0] == false);
    REQUIRE(pos.castling[1] == false);
    REQUIRE(pos.castling[2] == false);
    REQUIRE(pos.castling[3] == false);
}

TEST_CASE("FEN parsing - partial castling rights", "[fen]") {
    Square::init_squares();
    Position pos = Position::create_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kq - 0 1");
    
    REQUIRE(pos.castling[0] == true);   // White kingside
    REQUIRE(pos.castling[1] == false);  // White queenside
    REQUIRE(pos.castling[2] == false);  // Black kingside  
    REQUIRE(pos.castling[3] == true);   // Black queenside
}

TEST_CASE("FEN parsing - black to move", "[fen]") {
    Square::init_squares();
    Position pos = Position::create_position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    
    REQUIRE(pos.white_to_move == false);
}

TEST_CASE("FEN parsing - en passant square", "[fen]") {
    Square::init_squares();
    Position pos = Position::create_position("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    
    REQUIRE(pos.en_passant_square != 0);
}