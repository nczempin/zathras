#pragma once

#include <string>
#include <cstdint>

#include "typedefs.h"
#include "Square.h"
#include "misc.h"

namespace Moves {

	class Move
	{
	public:
		Move() {}
		//		Move(int8_t piece, uint8_t from, uint8_t to, int8_t captured, bool en_passant_capture);
		Move(uint8_t from, uint8_t to) :from(from), to(to) {}


		inline uint8_t get_from() const {
			return this->from;
		}
		inline uint8_t get_to() const {
			return this->to;
		}
		/*int8_t get_moving_piece() const;

		inline void set_moving_piece(int8_t moving) {
			this->moving = moving;
		}*/

		//void set_captured(int8_t taken);

		inline void set_from(uint8_t from) {
			this->from = from;
		}
		inline void set_to(uint8_t to) {
			this->to = to;
		}
		
		//int value{ 0 };



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
		square_t from = 0;
		square_t to = 0;
		//piece_t moving = 0;
		//piece_t captured = 0; //TODO maybe also move this out
		//piece_t promoted_to = 0;

		//bool en_passant_capture = false;



	};

	static string to_string(Move move) {
		/*static const string pieces("-PNBRQK");
		char p = pieces[moving > 0 ? moving : -moving];
		string moving_string = string(1, p);*/

		string retval = Positions::Square::mailbox_index_to_square(move.get_from());
		retval += Positions::Square::mailbox_index_to_square(move.get_to());


		return retval;
	}

}