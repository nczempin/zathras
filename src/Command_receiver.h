/*
 * Command_receiver.h
 *
 *  Created on: Mar 31, 2016
 *      Author: nczempin
 */

#ifndef COMMAND_RECEIVER_H_
#define COMMAND_RECEIVER_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Command_receiver
{
public:
  Command_receiver();
  virtual ~Command_receiver();
  void setArguments(vector<string> arguments)
  {
    this->arguments = arguments;
  }
  vector<string> getArguments()
  {
    return arguments;
  }
private:
  vector<string> arguments;
};

#endif /* COMMAND_RECEIVER_H_ */
