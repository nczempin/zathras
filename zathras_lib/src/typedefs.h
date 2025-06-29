#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <functional>
#include "Square.h"


using namespace std;

enum Move_type  {
	NONE,
	EN_PASSANT,
	PROMOTION_QUEEN,
	PROMOTION_ROOK,
	PROMOTION_BISHOP,
	PROMOTION_KNIGHT
};



using bb = uint_fast64_t;
//using square_t = uint8_t; //TODO this hides definition in Square.h

using square_t = Positions::Squares;

using move_type_t = Move_type;

using bitboard_set = array<bb, 64>; // one bitboard for every starting square
using square_visitor = function<void(square_t)>;
//using move_visitor = function<void(int8_t moving, uint8_t from, uint8_t to, int8_t captured, int8_t promoted_to)>;
using move_visitor = function<void(square_t from, square_t to, move_type_t move_type)>;

using piece_t = int8_t;

