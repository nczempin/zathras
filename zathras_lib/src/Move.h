#pragma once

#include <string>
#include <cstdint>

#include "typedefs.h"
#include "square.h"
#include "piece.h"
#include "misc.h"

namespace zathras_lib::moves {

	using positions::Squares;
	using positions::square_t;
	using positions::Piece;



	using Move = uint16_t;


	// promoted piece: 2 (0 = queen, 1 = rook, 2 = bishop, 3 = knight)
	// from: 6
	// MSB ^
	// LSB V
	// is_promotion: 1
	// is_en_passant: 1
	// to: 6
	//TODO when we add castling, bits 6 & 7 will have to be re-used


	inline constexpr square_t get_from(Move mm) {
		return static_cast<square_t>((mm >> 8) & 0b111111); //TODO just AND and SHIFT it?
	}
	inline constexpr square_t get_to(Move mm) {
		return static_cast<square_t>(mm & 0b111111); //TODO just AND it?
	}

	inline constexpr bool is_en_passant(Move mm) {
		return static_cast<bool>((mm >> 6)&1);// will need to change if we add CASTLING
	}
	inline constexpr bool is_promotion(Move mm) {
		return static_cast<bool>(mm >> 7); // will need to change if we add CASTLING
	}
	inline constexpr piece_t get_promoted(Move mm) {
		piece_t tmp = Piece::QUEEN - (mm >> 14 & 0b11);
		// 0 -> queen
		// 1 -> rook
		// 2 -> bishop
		// 3 -> knight
		return tmp;
	}

	//void set_captured(int8_t taken);

	inline void set_from(Move& mm, square_t from) {
		mm = (0b1100000011111111 & mm) | from << 8; // adding faster? probably not
	}
	inline void set_to(Move& mm, square_t to) {
		mm = (0b1111111111000000 & mm) | to; // adding faster? probably not
	}
	inline void set_en_passant(Move& mm, bool en_passant) {
		//TODO avoid the if?
		if (en_passant) {
			mm |= 0b0000000001000000;
		}
		else {
			mm &= 0b1111111110111111;
		}
		//		mm = (0b1111111110111111 & mm) | en_passant << 6;
	}




	//bool is_en_passant_capture() const {
	//	return this->move_type == EN_PASSANT;
	//}
	//void set_en_passant_capture(bool en_passant_capture) {
	//	this->move_type = en_passant_capture ? EN_PASSANT : NONE;
	//}

	/*int8_t get_promoted_to() const {
		return promoted_to;
	}

	void set_promoted_to(int8_t promotedTo = 0) {
		promoted_to = promotedTo;
	}

	int8_t get_captured() const {
		return captured;
	}*/








	static std::string to_string(Move move) {
		/*static const string pieces("-PNBRQK");
		char p = pieces[moving > 0 ? moving : -moving];
		string moving_string = string(1, p);*/

		std::string retval = positions::Square::mailbox_index_to_square(get_from(move));
		retval += positions::Square::mailbox_index_to_square(get_to(move));


		return retval;
	}

}
