/*
 * Move.h
 *
 *  Created on: 20.04.2016
 *      Author: admin
 */

#ifndef MOVE_H_
#define MOVE_H_

#include "typedefs.h"
#include "Square.h"
#include <string>
using namespace std;

class Move
{
public:
  Move();
  Move(int8_t piece, uint8_t from, uint8_t to, int8_t captured,
      uint8_t en_passant);
  uint8_t get_from();
  uint8_t get_to() const;
  string to_string() const;
  int8_t get_moving_piece() const;

  void set_moving_piece(int8_t moving);
  int8_t get_taken_piece() const;

  void set_taken_piece(int8_t taken);
  void set_from(uint8_t from);
  void set_to(uint8_t to);

  uint8_t get_en_passant() const
  {
    return en_passant;
  }

  void set_en_passant(uint8_t en_passant)
  {
    en_passant = en_passant;
  }

  int8_t get_captured() const
  {
    return captured;
  }

  void set_captured(int8_t captured = 0)
  {
    this->captured = captured;
  }

  bool cleared_queenside_castling = false;
  bool cleared_kingside_castling = false;

private:
  uint8_t from = 0;
  uint8_t to = 0;
  int8_t moving = 0;
  int8_t captured = 0;
  uint8_t en_passant;
};

#endif /* MOVE_H_ */
