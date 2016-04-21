/*
 * Move.cpp
 *
 *  Created on: 20.04.2016
 *      Author: admin
 */

#include "Move.h"

Move::Move(int piece, bb from, bb to, int captured = 0) :
    from(from), to(to), moving(piece), taken(captured)
{
  if (piece == 0) {
    throw 45;
  }
}

Move::~Move()
{
  // TODO Auto-generated destructor stub
}

