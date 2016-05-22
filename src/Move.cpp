/*
 * Move.cpp
 *
 *  Created on: 20.04.2016
 *      Author: admin
 */

#include "Move.h"
#include <iostream>
#include <string>

using namespace std;

Move::Move(int8_t moving = 0, uint8_t from = 0, uint8_t to = 0,
    int8_t captured = 0, bool en_passant_capture = false) :
    from(from), to(to), moving(moving), captured(captured), en_passant_capture(
        en_passant_capture)
{
  if (en_passant_capture) {
    if (moving == 1 || moving == -1) {

    } else {
      cerr << "non-pawn e. p." << endl;
      throw 123;
    }
  }

}
Move::Move() :
    from(0), to(0), moving(0), captured(0), en_passant_capture(false)
{
}

uint8_t Move::get_from()
{
  return from;
}
uint8_t Move::get_to() const
{
  return to;
}
//
//string Move::to_string() const
//{
////cout << moving << " " << from << " " << " " << to << " " << taken << endl;
//  static const string pieces("-PNBRQK");
//  char p = pieces[moving > 0 ? moving : -moving];
//  string moving_string = string(1, p);
//  string retval = moving_string + Square::mailbox_index_to_square(from);
//  retval += (captured != 0) ? "x" : "-";
//  retval += Square::mailbox_index_to_square(to);
//  if (captured) {
//    const signed char captured_abs = captured > 0 ? captured : -captured;
//    string captured_string = string(1, pieces[captured_abs]);
//    retval += " (" + captured_string + ")";
//  }
//  if (en_passant != 0) {
//    retval += " e. p.";
//  }
//  return retval;
//}
string Move::to_string() const
{
//cout << moving << " " << from << " " << " " << to << " " << taken << endl;
  static const string pieces("-PNBRQK");
  char p = pieces[moving > 0 ? moving : -moving];
  string moving_string = string(1, p);
  string retval = Square::mailbox_index_to_square(from);
  retval += Square::mailbox_index_to_square(to);
  uint8_t promoted = pieces[promoted_to > 0 ? promoted_to : -promoted_to];
  retval += promoted;
  retval += this->cleared_kingside_castling ? "#" : ".";
  retval += this->cleared_queenside_castling ? "#" : ".";
  if (en_passant_capture) {
    retval += " e. p.";
  }

  return retval;
}
int8_t Move::get_moving_piece() const
{
  return moving;
}

void Move::set_moving_piece(int8_t moving)
{
  int8_t moving_abs = moving > 0 ? moving : -moving;
  if (moving_abs > 6 || moving_abs == 0) {
    cerr << "invalid move set: moving piece: "
        << std::to_string((int) moving_abs) << endl;
    throw moving_abs;
  }
  this->moving = moving;
}


void Move::set_captured(int8_t taken)
{
  int8_t captured_abs = taken > 0 ? taken : -taken;
  if (captured_abs > 6) {
    cerr << "invalid move set: captured piece: " << taken << endl;
    throw captured_abs;
  }
  this->captured = taken;
}

void Move::set_from(uint8_t from)
{
  this->from = from;
}

void Move::set_to(uint8_t to)
{
  this->to = to;
}

bool Move::is_en_passant_capture() const
{
  return en_passant_capture;
}

void Move::set_en_passant_capture(bool epc)
{
  en_passant_capture = epc;
}
