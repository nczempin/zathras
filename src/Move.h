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
  Move(int8_t piece, uint8_t from, uint8_t to, int8_t captured);
  uint8_t get_from();
  uint8_t get_to() const;
  string to_string() const;
  int8_t get_moving_piece() const;

  void set_moving_piece(int8_t moving);
  int8_t get_taken_piece() const;

  void set_taken_piece(int8_t taken);
private:
  Move();
  uint8_t from;
  uint8_t to;
  int8_t moving;
  int8_t captured;
};

#endif /* MOVE_H_ */
