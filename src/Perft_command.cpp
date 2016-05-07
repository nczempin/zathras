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
  Move_container move_container = mg.generate_moves(pp, depth);
  array<Move, Move_container::SIZE> moves = move_container.get_moves();
  //  if (depth == 1) {
//    return moves.size();
//  }
  int total_result = 0;
  size_t size = move_container.size();
//  cout << "moves.size: " << size << endl;
//  cout << "perft::perft" << endl;
//  for (size_t i = 0; i < size; ++i) {
//    Move move = moves[i];
//    cout << move.to_string() << endl;
//  }
  for (size_t i = 0; i < size; ++i) {
    Move& move = moves[i];
//    cout << "(mm: perft@" << depth << "): " << move.to_string() << "-->"
//        << endl;
//    cout << *pp << endl;
    pp->make_move(move);
//    cout << *pp << endl;
//    cout << "(bc: perft@" << depth << "): " << move.to_string() << "-->"
//        << endl;
    if (mg.is_in_check(!pp->white_to_move)) {
      //cout << move.to_string() << "******was illegal; unmaking*******" << endl;
      pp->unmake_move(move);
      continue;
    }
    if (depth == 1) {
      ++total_result;
    } else {
      int perft_result = perft(depth - 1);
      total_result += perft_result;
    }
//    cout << "(bu: perft@" << depth << "): " << move.to_string() << "-->"
//        << endl;
    pp->unmake_move(move);
//    cout << "(um: perft@" << depth << "): " << move.to_string() << "-->"
//        << endl;

//    cout << "after unmake_move:" << endl;
//    cout << p << endl;
  }
  return total_result;
}

void Perft_command::execute()
{
  vector<string> path = receiver->getArguments();
  int depth = 6; //TODO get this from arguments, but use a reasonable default
  Position position;
  position = Position::create_position(
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq –");
  position = Position::create_start_position();
  cout << "Perft " << depth << " for this position: " << endl;
  cout << position << endl;
  mg.pregenerate_moves();

  ///////////////////////////////
  int total_result = 0;
  //cout << "b4 gen" << endl;
  pp = make_shared<Position>(position);
  Move_container move_container = mg.generate_moves(pp, depth);
  array<Move, Move_container::SIZE> moves = move_container.get_moves();
  //cout << "after gen" << endl;
  size_t size = move_container.size();
//  cout << "moves.size: " << size << endl;
  if (depth == 0) {
    total_result = size;
  } else {
//    for (size_t i = 0; i < size; ++i) {
//      Move move = moves[i];
//      cout << move.to_string() << endl;
//    }
    for (size_t i = 0; i < size; ++i) {
      Move& move = moves[i];
      int8_t moving = move.get_moving_piece();
      int8_t moving_abs = moving > 0 ? moving : -moving;
      if (moving_abs > 6 || moving_abs == 0) {
        cerr << "size: " << size << endl;
        cerr << "invalid move created: moving piece: " << moving << endl;
        throw moving_abs;
      }
//      cout << "on move in perft_execute: " << p.white_to_move << endl;
//      cout.flush();
      string s = move.to_string();
//      cout << "(make move) " << s << endl;
      //cout << s << "******************" << endl;
      pp->make_move(move);
      if (mg.is_in_check(!pp->white_to_move)) {
        //cout << "******was illegal; unmaking*******" << endl;
        pp->unmake_move(move);
        continue;
      } else {
        // cout << move.to_string() << " is not check" << endl;
      }
//      cout.flush();
//    cout << "after make_move:" << endl;
//    cout << p << endl;
      int perft_result = perft(depth - 1);
      s = move.to_string();
      cout << s << ": " << perft_result << endl;
      total_result += perft_result;
      pp->unmake_move(move);
//    cout << "after unmake_move:" << endl;
//    cout << p << endl;
    }
  }
  cout << endl << "Perft " << depth << " result: " << total_result << endl;
}
