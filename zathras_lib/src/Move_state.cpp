/*
 * Move_state.cpp
 *
 *  Created on: 02.12.2017
 *      Author: nczempin
 */
#include "Move_state.h"
namespace Moves {

	Move_state::Move_state() :en_passant_square(-1), cleared_kingside_castling(false), cleared_queenside_castling(false) {
	}

	Move_state::~Move_state() {
		// TODO Auto-generated destructor stub
	}
}
