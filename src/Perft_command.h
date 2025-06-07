/*
 * perft_command.h
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#ifndef PERFT_COMMAND_H_
#define PERFT_COMMAND_H_

#include <memory>
#include <stack>


#include "Abstract_command.h"
#include "Command_receiver.h"
#include "Position.h"
#include "Move_generator.h"
#include "Move.h"

namespace zathras::interface {
	class Perft_command : public Abstract_command {
	public:
		Perft_command(positions::Position position, size_t depth) :position(position),depth(depth) {}
		virtual ~Perft_command();
		void execute() override;

	private:
		uint64_t perft(uint8_t depth);
		string format_large_number(int nps);

		shared_ptr<positions::Position> pp;
		zathras_lib::moves::Move_generator mg;
		size_t illegal_moves_generated = 0;
		size_t depth = 6;
		positions::Position position;

		std::stack<zathras_lib::moves::Move> s;
	};
}
#endif /* PERFT_COMMAND_H_ */
