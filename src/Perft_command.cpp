/*
 * Perft_command.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#include "Perft_command.h"

Perft_command::Perft_command(shared_ptr<Command_receiver> receiver) :
    Abstract_command(receiver)
{
  // TODO Auto-generated constructor stub

}

Perft_command::~Perft_command()
{
  // TODO Auto-generated destructor stub
}

void Perft_command::do_long_option(bool flag, string name, string argument)
{
  if (flag) {
    cout << "flag" << endl;
  }
  cout << "name: " << name << endl;
  cout << "argument: " << argument << endl;
}

string Perft_command::get_short_options()
{
  return "t";
}
vector<option> Perft_command::get_long_options()
{
  vector<option> retval =
    {
      { "textconv", no_argument, 0, 0 } };
  return retval;
}

void Perft_command::do_short_option(int c, string argument)
{
  switch (c) {
  case 'w':
    //cout << "option -w" << endl;
    break;
  case 't':
    //  cout << "option -t" << endl;
    //  set_option_type(true);
    break;
  case '?':
    /* getopt_long already printed an error message. */
    break;
  default:
    throw 129; //TODO is this specific or generic?
  }
}

void Perft_command::execute()
{
  vector<string> path = receiver->getArguments();
}
