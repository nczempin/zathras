/*
 * Move.cpp
 *
 *  Created on: 20.04.2016
 *      Author: admin
 */

#include "Move.h"
#include <iostream>

using namespace std;

Move::Move(int8_t moving, uint8_t from, uint8_t to, int8_t captured = 0) :
    from(from), to(to), moving(moving), captured(captured)
{
  int8_t moving_abs = moving > 0 ? moving : -moving;
  if (moving_abs > 6) {
    cerr << "invalid move created: moving piece: " << moving << endl;
    throw moving_abs;
  }
  int8_t captured_abs = captured > 0 ? captured : -captured;
  if (captured_abs > 6) {
    if (moving_abs > 6) {
      cerr << "invalid move created: captured piece: " << captured << endl;
      throw captured_abs;
    }

  }
}

uint8_t Move::get_from()
{
  return from;
}
uint8_t Move::get_to() const
{
  return to;
}
string Move::to_string() const
{
  //cout << moving << " " << from << " " << " " << to << " " << taken << endl;
  static const string pieces("-PNBRQK");
  char p = pieces[moving > 0 ? moving : -moving];
  string moving_string = string(1, p);
  string retval = moving_string + Square::mailbox_index_to_square(from);
  retval += (captured != 0) ? "x" : "-";
  retval += Square::mailbox_index_to_square(to);
  if (captured) {
    const signed char captured_abs = captured > 0 ? captured : -captured;
    string captured_string = string(1, pieces[captured_abs]);
    retval += " (" + captured_string + ")";
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
    cerr << "invalid move created: moving piece: " << moving << endl;
    throw moving_abs;
  }
  int8_t captured_abs = captured > 0 ? captured : -captured;
  if (captured_abs > 6) {
    cerr << "invalid move created: captured piece: " << captured << endl;
    throw captured_abs;
  }
  this->moving = moving;
}

int8_t Move::get_taken_piece() const
{
  return captured;
}

void Move::set_taken_piece(int8_t taken)
{
  int8_t moving_abs = moving > 0 ? moving : -moving;
  if (moving_abs > 6 || moving_abs == 0) {
    cerr << "invalid move created: moving piece: " << moving << endl;
    throw moving_abs;
  }
  int8_t captured_abs = captured > 0 ? captured : -captured;
  if (captured_abs > 6) {
    cerr << "invalid move created: captured piece: " << captured << endl;
    throw captured_abs;
  }
  this->captured = taken;
}
