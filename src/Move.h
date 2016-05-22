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
      bool en_passant_capture);
  uint8_t get_from();
  uint8_t get_to() const;
  string to_string() const;
  int8_t get_moving_piece() const;

  void set_moving_piece(int8_t moving);

  void set_captured(int8_t taken);
  void set_from(uint8_t from);
  void set_to(uint8_t to);

  uint8_t get_en_passant_square() const
  {
    return en_passant_square;
  }

  void set_en_passant_square(uint8_t en_passant)
  {
    en_passant = en_passant;
  }



  bool is_en_passant_capture() const;
  void set_en_passant_capture(bool enPassantCapture = false);

  int8_t get_promoted_to() const
  {
    return promoted_to;
  }

  void set_promoted_to(int8_t promotedTo = 0)
  {
    promoted_to = promotedTo;
  }

  int8_t get_captured() const
  {
    return captured;
  }

  bool cleared_queenside_castling = false;
  bool cleared_kingside_castling = false;

private:
  uint8_t from = 0;
  uint8_t to = 0;
  int8_t moving = 0;
  int8_t captured = 0;
  uint8_t en_passant_square = 0; //TODO file would be sufficient
  bool en_passant_capture = false;
  int8_t promoted_to = 0;

};

#endif /* MOVE_H_ */
