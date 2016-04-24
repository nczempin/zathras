/*
 * Perft_command.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#include "Perft_command.h"

#include <getopt.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Position.h"
#include "Move_generator.h"

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

int Perft_command::perft(int depth)
{
  if (depth == 0) {
    return 1;
  }
  vector<Move> moves = mg.generate_moves(p);
  if (depth == 1) {
    return moves.size();
  }
  int total_result = 0;
  for (auto &move : moves) {
    //cout << move.to_string() << "-->" << endl;
    p.make_move(move);
//    cout << "after make_move:" << endl;
//    cout << p << endl;

    if (depth == 1) {
      ++total_result;
    } else {
      int perft_result = perft(depth - 1);
      total_result += perft_result;
    }
    p.unmake_move(move);
//    cout << "after unmake_move:" << endl;
//    cout << p << endl;
  }
  return total_result;
}

void Perft_command::execute()
{
  vector<string> path = receiver->getArguments();
  int depth = 6; //TODO get this from arguments, but use a reasonable default
  p = Position::create_position("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq –");
  p = Position::create_start_position();
  cout << "Perft " << depth << " for this position: " << endl;
  cout << p << endl;

  mg.pregenerate_moves();

  ///////////////////////////////

  vector<Move> moves = mg.generate_moves(p);
  int total_result = 0;
  for (auto &move : moves) {
    string s = move.to_string();

    //cout << s << "---------->" << endl;

    p.make_move(move);
//    cout << "after make_move:" << endl;
//    cout << p << endl;
    int perft_result = perft(depth - 1);
    cout << s << ": " << perft_result << endl;
    total_result += perft_result;
    p.unmake_move(move);
//    cout << "after unmake_move:" << endl;
//    cout << p << endl;
  }
  cout << endl << "Perft result: " << total_result << endl;
}
