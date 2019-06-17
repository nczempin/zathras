/*
 * Move.cpp
 *
 *  Created on: 20.04.2016
 *      Author: nczempin
 */

#include "Move.h"
#include <iostream>
#include <string>

using namespace std;

Move::Move(int8_t moving, uint8_t from, uint8_t to, int8_t captured, bool en_passant_capture) :
    from(from), to(to), moving(moving), captured(captured), en_passant_capture(en_passant_capture) {

}
Move::Move() :
    from(0), to(0), moving(0), captured(0), en_passant_capture(false) {
}


string Move::to_string() const {
  static const string pieces("-PNBRQK");
  char p = pieces[moving > 0 ? moving : -moving];
  string moving_string = string(1, p);
  string retval = Square::mailbox_index_to_square(from);
  retval += Square::mailbox_index_to_square(to);
  uint8_t promoted = pieces[promoted_to > 0 ? promoted_to : -promoted_to];
  retval += promoted;
//  retval += this->cleared_kingside_castling ? "#" : ".";
//  retval += this->cleared_queenside_castling ? "#" : ".";
  if (en_passant_capture) {
    retval += " e. p.";
  }

  return retval;
}
int8_t Move::get_moving_piece() const {
  return moving;
}

void Move::set_captured(int8_t taken) {
  this->captured = taken;
}

bool Move::is_en_passant_capture() const {
  return en_passant_capture;
}

void Move::set_en_passant_capture(bool epc) {
  en_passant_capture = epc;
}
