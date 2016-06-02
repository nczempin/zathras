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

class Perft_command: public Abstract_command
{
public:
  Perft_command(shared_ptr<Command_receiver> receiver);
  virtual ~Perft_command();
  void execute();
  string get_short_options();
  vector<option> get_long_options(); //TODO dependency on getopt
  void do_long_option(bool flag, string name, string argument);
  void do_short_option(int c, string argument);

private:
  uint64_t perft(uint8_t depth);

  shared_ptr<Position> pp;
  Move_generator mg;
};

#endif /* PERFT_COMMAND_H_ */
