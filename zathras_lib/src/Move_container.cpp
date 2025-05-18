#include <array>
#include <iostream>
//#include <span>

#include "Move_container.h"
#include "Piece.h"

namespace zathras_lib::moves {

	array<Move_container, CONTAINER_COUNT> Move_container::container_pool; // TODO perhaps better to use vector, once we get to depth 10

	Move_container::Move_container()
	{
		/*Move m;
		for (size_t i = 0; i < SIZE; ++i) {
			container[i] = m;
		}*/
		index = 0;
	}

	Move_container::~Move_container()
	{
		// TODO Auto-generated destructor stub
	}

	Move_container& Move_container::get(size_t index)
	{
		return container_pool[index];
	}

	/*void Move_container::add_move(const int8_t& moving, const uint8_t& from, const uint8_t& to,
		const int8_t& captured, const bool& en_passant_capture, const int8_t& promoted_to)
	{
		Move& m = container[index];
		m.set_moving_piece(moving);
		m.set_from(from);
		m.set_to(to);
		m.set_captured(captured);
		m.set_en_passant_capture(en_passant_capture);

		m.set_promoted_to(promoted_to);
		++index;
	}*/
	void Move_container::add_move(const square_t& from, const square_t& to, const move_type_t& move_type)
	{
		Move& m = container[index];
		set_from(m, from);
		set_to(m, to);
		auto is_ep = move_type == EN_PASSANT;
		if (move_type) {
			set_en_passant(m, is_ep);
		}
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