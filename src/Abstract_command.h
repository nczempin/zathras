/*
 * Abstract_command.h
 *
 *  Created on: Mar 31, 2016
 *      Author: nczempin
 */

#ifndef ABSTRACT_COMMAND_H_
#define ABSTRACT_COMMAND_H_

#include <string>
#include <memory>
#include <vector>

#include "Command.h"
#include "Command_receiver.h"


using namespace std;

class Abstract_command : public Command
{
public:
  Abstract_command();
  virtual ~Abstract_command();
  virtual void execute() = 0;
  
};

#endif /* ABSTRACT_COMMAND_H_ */
