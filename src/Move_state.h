/*
 * Move_state.h
 *
 *  Created on: 02.12.2017
 *      Author: nczempin
 */

#ifndef MOVE_STATE_H_
#define MOVE_STATE_H_

#include "typedefs.h"

class Move_state {
public:
	Move_state();
	virtual ~Move_state();

	bool is_cleared_kingside_castling() const {
		return cleared_kingside_castling;
	}

	void set_cleared_kingside_castling(bool b) {
		this->cleared_kingside_castling = b;
	}
	void set_cleared_queenside_castling(bool b) {
		cleared_queenside_castling = b;
	}

	bool is_cleared_queenside_castling() const {
		return cleared_queenside_castling;
	}

	uint8_t get_en_passant_square() const {
		return en_passant_square;
	}

	void set_en_passant_square(uint8_t en_passant) {
		this->en_passant_square = en_passant;
	}

private:
	square_t en_passant_square = 0; //TODO file would be sufficient
	bool cleared_kingside_castling = false;
	bool cleared_queenside_castling = false;

};

#endif /* MOVE_STATE_H_ */
