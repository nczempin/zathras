/*
 * Perft_command.h
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#ifndef PERFT_COMMAND_H_
#define PERFT_COMMAND_H_

#include "Abstract_command.h"
#include "Command_receiver.h"
#include "Position.h"
#include "Move_generator.h"
#include <memory>

class Perft_command : public Abstract_command {
public:
	Perft_command();
	virtual ~Perft_command();
	void execute();

private:
	uint64_t perft(uint8_t depth);
	string format_large_number(int nps);

	shared_ptr<Position> pp;
	Move_generator mg;
};

#endif /* PERFT_COMMAND_H_ */
