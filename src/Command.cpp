/*
 * Command.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#include "Command.h"

Command::Command(shared_ptr<Command_receiver> receiver) :
    receiver(receiver)
{
}


Command::~Command()
{
  // TODO Auto-generated destructor stub
}

