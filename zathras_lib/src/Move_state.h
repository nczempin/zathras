/*
 * Move_state.h
 *
 *  Created on: 02.12.2017
 *      Author: nczempin
 */

#ifndef MOVE_STATE_H_
#define MOVE_STATE_H_

#include <bitset>
#include "typedefs.h"

namespace Moves {
	class Move_state {
	public:
		Move_state();
		~Move_state();

		piece_t captured = 0;
		bb prev_en_passant_square = 0;
		std::bitset<4> prev_castling;
	};
}
#endif /* MOVE_STATE_H_ */
