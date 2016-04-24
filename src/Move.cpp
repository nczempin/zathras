/*
 * Move.cpp
 *
 *  Created on: 20.04.2016
 *      Author: admin
 */

#include "Move.h"
#include <iostream>

using namespace std;

Move::Move(int8_t piece, uint8_t from, uint8_t to, int8_t captured = 0) :
    from(from), to(to), moving(piece), taken(captured)
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
string Move::to_string() const
{
  //cout << moving << " " << from << " " << " " << to << " " << taken << endl;
  static const string pieces("- NBRQK");
  char p = pieces[moving > 0 ? moving : -moving];
  string moving_string = string(1, p);
  string retval = moving_string + Square::mailbox_index_to_square(from);
  retval += (taken != 0) ? "x" : "-";
  retval += Square::mailbox_index_to_square(to);
  if (taken) {
    retval += " (" + string(1, pieces[taken]) + ")";
  }
  return retval;
}
int8_t Move::get_moving_piece() const
{
  return moving;
}

void Move::set_moving_piece(int8_t moving)
{
  this->moving = moving;
}

int8_t Move::get_taken_piece() const
{
  return taken;
}

void Move::set_taken_piece(int8_t taken)
{
  this->taken = taken;
}
