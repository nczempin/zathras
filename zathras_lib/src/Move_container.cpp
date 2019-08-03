#include <array>
#include <iostream>
//#include <span>

#include "Move_container.h"
#include "Piece.h"

namespace Moves {

	//array<Move_container, CONTAINER_COUNT> Move_container::container_pool; // TODO perhaps better to use vector, once we get to depth 10

	
	//void Move_container::add_move(const square_t& from, const square_t& to, const move_type_t& move_type)
	//{
	//	Move& m = container[index];
	//	set_from(m, from);
	//	set_to(m, to);
	//	set_move_type(m, move_type);
	//	//auto is_ep = move_type == EN_PASSANT;
	//	//if (move_type) {
	//	//	set_en_passant(m, is_ep);
	//	//}
	//	++index;
	//}

	//void Move_container::reset()
	//{
	//	index = 0;
	//}

	//size_t Move_container::size() const
	//{
	//	return index;
	//}

	//move_container_t Move_container::get_moves()const
	//{
	//	return container; //TODO unsafe?
	//}
}