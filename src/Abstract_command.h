/*
 * Abstract_command.h
 *
 *  Created on: Mar 31, 2016
 *      Author: nczempin
 */

#ifndef ABSTRACT_COMMAND_H_
#define ABSTRACT_COMMAND_H_

#include <string>
#include <getopt.h>
#include <memory>
#include <vector>

#include "Command.h"
#include "Command_receiver.h"


using namespace std;

class Abstract_command : public Command
{
public:
  Abstract_command(shared_ptr<Command_receiver> receiver);
  virtual ~Abstract_command();
  virtual void execute() = 0;
  virtual string get_short_options() = 0;
	virtual vector<option> get_long_options() = 0; //TODO dependency on getopt
  virtual void do_long_option(bool flag, string name, string argument) = 0;
  virtual void do_short_option(int c, string argument) = 0;
 // virtual shared_ptr<CommandParameter> createCommandParameter() = 0;

};

#endif /* ABSTRACT_COMMAND_H_ */
