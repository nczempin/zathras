/*
 * Command.h
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#ifndef COMMAND_H_
#define COMMAND_H_
#include <memory>

#include "Command_receiver.h"

using namespace std;

class Command
{
public:
  Command(shared_ptr<Command_receiver> receiver);
  virtual ~Command();
  virtual void execute() = 0;
protected:
  shared_ptr<Command_receiver> receiver;

};

#endif /* COMMAND_H_ */
