/*
 * Abstract_mygit_command.cpp
 *
 *  Created on: Mar 31, 2016
 *      Author: nczempin
 */

#include "Abstract_command.h"

Abstract_command::Abstract_command(shared_ptr<Command_receiver> receiver) :
    Command(receiver)
{
}

Abstract_command::~Abstract_command()
{
  // TODO Auto-generated destructor stub
}

