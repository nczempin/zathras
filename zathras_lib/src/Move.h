#pragma once

#include <string>
#include <cstdint>

#include "typedefs.h"
#include "Square.h"
#include "misc.h"

namespace Moves {

	using Positions::Squares;
	using Positions::square_t;



	class Move
	{
	public:
		Move() = default;
		Move(square_t from, square_t to, move_type_t move_type) :from(from), to(to), move_type(move_type) {
		}


		inline square_t get_from() const {
			return this->from;
		}
		inline square_t get_to() const {
			return this->to;
		}

		inline move_type_t get_move_type() const {
			return this->move_type;
		}

		inline bool is_null() const {
			return this->from == Squares::A1 && this->to == Squares::A1 && this->move_type == NONE;
		}

		inline bool is_promotion() const {
			return this->move_type == PROMOTION_QUEEN ||
			       this->move_type == PROMOTION_ROOK ||
			       this->move_type == PROMOTION_BISHOP ||
			       this->move_type == PROMOTION_KNIGHT;
		}

		inline void set_from(square_t from) {
			this->from = from;
		}
		inline void set_to(square_t to) {
			this->to = to;
		}
		inline void set_move_type(move_type_t move_type) {
			this->move_type = move_type;
		}

	private:
		Squares from = Squares::A1;
		Squares to = Squares::A1;
		Move_type move_type = NONE;
	};

	inline std::string to_string(Move move) {
		if (move.is_null()) {
			return "0000";
		}

		std::string retval = Positions::Square::mailbox_index_to_square(move.get_from());
		retval += Positions::Square::mailbox_index_to_square(move.get_to());

		// Add promotion notation
		switch (move.get_move_type()) {
			case PROMOTION_QUEEN:
				retval += "q";
				break;
			case PROMOTION_ROOK:
				retval += "r";
				break;
			case PROMOTION_BISHOP:
				retval += "b";
				break;
			case PROMOTION_KNIGHT:
				retval += "n";
				break;
			default:
				// No suffix for normal moves
				break;
		}

		return retval;
	}

}