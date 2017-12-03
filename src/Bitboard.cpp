/*
 * Bitboard.cpp
 *
 *  Created on: 24.11.2017
 *      Author: nczempin
 */

#include "Bitboard.h"

#include "Move_generator.h" //TODO cyclic dependency

Bitboard::Bitboard() {
	// TODO Auto-generated constructor stub

}

Bitboard::~Bitboard() {
	// TODO Auto-generated destructor stub
}
void Bitboard::visit_bitboard(const bb my_bb, const square_visitor f) {
	bb tmp = my_bb;
	uint8_t coord = 0;
	uint8_t l = 0;
	while (true) {
		l = ffs(tmp);
		if (l == 0) {
			return;
		}
		coord = look_up(l);

		f(coord);
		tmp &= tmp - 1; //clear LS1B
	}
}


const bitboard_set Bitboard::knight_moves = Move_generator::pregenerate_knight_moves();

const bitboard_set Bitboard::king_moves = Move_generator::pregenerate_king_moves();
const bitboard_set Bitboard::bishop_moves = Move_generator::pregenerate_bishop_moves();
const bitboard_set Bitboard::rook_moves = Move_generator::pregenerate_rook_moves();
const bitboard_set Bitboard::queen_moves = Move_generator::pregenerate_queen_moves();
const bitboard_set Bitboard::white_pawn_no_capture_moves =
		Move_generator::pregenerate_white_pawn_no_capture_moves();
const bitboard_set Bitboard::black_pawn_no_capture_moves =
		Move_generator::pregenerate_black_pawn_no_capture_moves();
const bitboard_set Bitboard::white_pawn_capture_moves =
		Move_generator::pregenerate_white_pawn_capture_moves();
const bitboard_set Bitboard::black_pawn_capture_moves =
		Move_generator::pregenerate_black_pawn_capture_moves();
