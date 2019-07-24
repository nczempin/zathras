/*
 * Bitboard.cpp
 *
 *  Created on: 24.11.2017
 *      Author: nczempin
 */

#include "Bitboard.h"
#include "typedefs.h"
#include <cassert>


#include "Move_generator.h" //TODO cyclic dependency


namespace Positions {
	Bitboard::Bitboard() noexcept {
		// TODO Auto-generated constructor stub

	}

	Bitboard::~Bitboard() {
		// TODO Auto-generated destructor stub
	}
	void Bitboard::visit_bitboard(const bb my_bb, const square_visitor& f) {
		if (my_bb == 0) {
			return;
		}
		bb tmp = my_bb;
		uint8_t coord = 0;
		uint8_t l = 0;
		while (tmp!=0) {
			l = ffs(tmp);
			//if (l == 0) {
			//	return;
			//}
			coord = look_up(l);
			uint8_t coord2 = look_up2(l);

			f(square_t(coord)); //TODO handle cast better
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
}