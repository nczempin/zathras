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
		Move() {
		}
		//		Move(int8_t piece, uint8_t from, uint8_t to, int8_t captured, bool en_passant_capture);
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


		//void set_captured(int8_t taken);

		inline void set_from(square_t from) {
			this->from = from;
		}
		inline void set_to(square_t to) {
			this->to = to;
		}
		inline void set_move_type(move_type_t move_type) {
			this->move_type = move_type;
		}
		



		/*bool is_en_passant_capture() const;
		void set_en_passant_capture(bool enPassantCapture);
*/
/*int8_t get_promoted_to() const {
	return promoted_to;
}

void set_promoted_to(int8_t promotedTo = 0) {
	promoted_to = promotedTo;
}

int8_t get_captured() const {
	return captured;
}*/


	private:
		Squares from;
		Squares to;
		Move_type move_type;
		//piece_t moving = 0;
		//piece_t captured = 0; //TODO maybe also move this out
		//piece_t promoted_to = 0;

		//bool en_passant_capture = false;



	};

	static std::string to_string(Move move) {
		/*static const string pieces("-PNBRQK");
		char p = pieces[moving > 0 ? moving : -moving];
		string moving_string = string(1, p);*/

		std::string retval = Positions::Square::mailbox_index_to_square(move.get_from());
		retval += Positions::Square::mailbox_index_to_square(move.get_to());


		return retval;
	}

}