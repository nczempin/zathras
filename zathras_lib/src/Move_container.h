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
	const size_t CONTAINER_COUNT = 32; //TODO this needs to be the maximum depth
	const size_t CONTAINER_SIZE = 127;
	using Positions::square_t;
	using move_container_t = array<Move, CONTAINER_SIZE>;
	//using move_container_t = vector<Move>; //TODO make this work? or make the array safer


	class Move_container
	{
	public:
		Move_container();
		virtual ~Move_container();

		

		static Move_container& get(size_t index);
//		void add_move(const int8_t& moving, const uint8_t& from, const uint8_t& to, const int8_t& captured, const bool& en_passant_capture, const int8_t& promoted_to);
		void add_move(const square_t& from, const square_t& to, const Move_type& move_type);
		void add_move(const Move& move) {
			add_move(
				//move.get_moving_piece(),
				move.get_from(),
				move.get_to(),
				move.get_move_type()
				/*,
				move.get_captured(), 
				move.is_en_passant_capture(), 
				move.get_promoted_to()*/
			);
		}
		void reset();
		size_t size()const;
		move_container_t get_moves() const;
	private:
		static array<Move_container, CONTAINER_COUNT> container_pool;
		move_container_t container;
		int index = 0;
	};
}
#endif /* MOVE_CONTAINER_H_ */
