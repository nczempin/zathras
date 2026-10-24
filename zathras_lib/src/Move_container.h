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
	const size_t CONTAINER_SIZE = 256;
	using Positions::square_t;
	using move_container_t = array<Move, CONTAINER_SIZE>;


	class Move_container
	{
	public:
		Move_container();
		~Move_container();

		void add_move(const square_t& from, const square_t& to, const Move_type& move_type);
		void add_move(const Move& move) {
			add_move(
				move.get_from(),
				move.get_to(),
				move.get_move_type()
			);
		}
		void reset();
		size_t size()const;
		move_container_t get_moves() const;
		const move_container_t& get_moves_ref() const { return container; }
		move_container_t& get_moves_ref() { return container; }
	private:
		move_container_t container;
		int index = 0;
	};
}
#endif /* MOVE_CONTAINER_H_ */
