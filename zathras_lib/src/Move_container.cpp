#include <array>
#include <iostream>
//#include <span>

#include "Move_container.h"
#include "Piece.h"

namespace Moves {

	Move_container::Move_container()
	{
		index = 0;
	}

	Move_container::~Move_container()
	{
	}

	void Move_container::add_move(const square_t& from, const square_t& to, const move_type_t& move_type)
	{
		Move& m = container[index];
		m.set_from(from);
		m.set_to(to);
		m.set_move_type(move_type);
		++index;
	}

	void Move_container::reset()
	{
		index = 0;
	}

	size_t Move_container::size() const
	{
		return index;
	}

	move_container_t Move_container::get_moves()const
	{
		return container; //TODO unsafe?
	}
}