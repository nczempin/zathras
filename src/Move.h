/*
 * Move.h
 *
 *  Created on: 20.04.2016
 *      Author: admin
 */

#ifndef MOVE_H_
#define MOVE_H_

#include "typedefs.h"

class Move {
public:
  Move(bb from, bb to);
  virtual ~Move();
  bb get_from()
  {
    return from;
  }
  bb get_to()
  {
    return to;
  }
private:
  bb from;
  bb to;
};

#endif /* MOVE_H_ */
