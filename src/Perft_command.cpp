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
//  cout << "perft depth: " << depth << endl;
  if (depth == 0) {
    return 1;
  }
  vector<Move> moves = mg.generate_moves(pp);
  //  if (depth == 1) {
//    return moves.size();
//  }
  int total_result = 0;
  size_t size = moves.size();
//  cout << "moves.size: " << size << endl;
//  cout << "perft::perft" << endl;
//  for (size_t i = 0; i < size; ++i) {
//    Move move = moves[i];
//    cout << move.to_string() << endl;
//  }
  for (size_t i = 0; i < size; ++i) {
    Move move = moves[i];
//    cout << "(perft@" << depth << "): " << move.to_string() << "-->" << endl;
    pp->make_move(move);
    if (!mg.is_in_check(!pp->white_to_move)) {
      if (depth == 1) {
        ++total_result;
      } else {
        int perft_result = perft(depth - 1);
        total_result += perft_result;
      }
    }
    pp->unmake_move(move);
//    cout << "after unmake_move:" << endl;
//    cout << p << endl;
  }
  return total_result;
}

void Perft_command::execute()
{
  vector<string> path = receiver->getArguments();
  int depth = 2; //TODO get this from arguments, but use a reasonable default
  Position position = Position::create_position("8/8/8/8/8/k7/8/K7 w - - 0 1");
  //p = Position::create_start_position();
  cout << "Perft " << depth << " for this position: " << endl;
  cout << position << endl;
  mg.pregenerate_moves();

  ///////////////////////////////
  int total_result = 0;
  //cout << "b4 gen" << endl;
  pp = make_shared<Position>(position);
  vector<Move> moves = mg.generate_moves(pp);
  //cout << "after gen" << endl;
  size_t size = moves.size();
//  cout << "moves.size: " << size << endl;
  if (depth == 0) {
    total_result = size;
  } else {
//    for (size_t i = 0; i < size; ++i) {
//      Move move = moves[i];
//      cout << move.to_string() << endl;
//    }
    for (size_t i = 0; i < size; ++i) {
      Move move = moves[i];
      int8_t moving = move.get_moving_piece();
      int8_t moving_abs = moving > 0 ? moving : -moving;
      if (moving_abs > 6 || moving_abs == 0) {
        cerr << "invalid move created: moving piece: " << moving << endl;
        throw moving_abs;
      }
//      cout << "on move in perft_execute: " << p.white_to_move << endl;
//      cout.flush();
      string s = move.to_string();
//      cout << "(made move) " << s << endl;
      pp->make_move(move);
      if (mg.is_in_check(!pp->white_to_move)) {
        pp->unmake_move(move);
        continue;
      }
//      cout.flush();
//    cout << "after make_move:" << endl;
//    cout << p << endl;
      int perft_result = perft(depth - 1);
      cout << s << ": " << perft_result << endl;
      total_result += perft_result;
      pp->unmake_move(move);
//    cout << "after unmake_move:" << endl;
//    cout << p << endl;
    }
  }
  cout << endl << "Perft " << depth << " result: " << total_result << endl;
}
