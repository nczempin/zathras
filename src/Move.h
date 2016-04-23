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
  Move(int piece, bb from, bb to, int captured);
  virtual ~Move();
  bb get_from();
  bb get_to() const;
  string to_string() const;
  int get_moving_piece() const;

  void set_moving_piece(int moving);
  int get_taken_piece() const;

  void set_taken_piece(int taken);
private:
  bb from;
  bb to;
  int moving;
  int taken;
};

#endif /* MOVE_H_ */
