/*
 * Move_container.h
 *
 *  Created on: Apr 28, 2016
 *      Author: nczempin
 */

#ifndef MOVE_CONTAINER_H_
#define MOVE_CONTAINER_H_

#include "Move.h"

#include <stddef.h>
#include <cstdint>
#include <vector>
#include <array>

namespace Moves {
	const size_t CONTAINER_COUNT = 99; //TODO this needs to be the maximum depth

	using Positions::square_t;


	class Move_container
	{
	public:
		Move_container();
		virtual ~Move_container();

		static const size_t SIZE = 127;

		static Move_container& get(size_t index);
//		void add_move(const int8_t& moving, const uint8_t& from, const uint8_t& to, const int8_t& captured, const bool& en_passant_capture, const int8_t& promoted_to);
		void add_move(const square_t& from, const square_t& to);
		void add_move(const Move& move) {
			add_move(
				//move.get_moving_piece(),
				move.get_from(),
				move.get_to()
				/*,
				move.get_captured(), 
				move.is_en_passant_capture(), 
				move.get_promoted_to()*/
			);
		}
		void reset();
		size_t size()const;
		array<Move, SIZE> get_moves() const;
	private:
		static array<Move_container, CONTAINER_COUNT> container_pool;
		array<Move, SIZE> container;
		int index = 0;
	};
}
#endif /* MOVE_CONTAINER_H_ */
