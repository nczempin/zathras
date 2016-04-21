/*
 * Move.cpp
 *
 *  Created on: 20.04.2016
 *      Author: admin
 */

#include "Move.h"

Move::Move(int piece, bb from, bb to, int captured) :
    from(from), to(to), moving(piece), taken(captured)
{
  // TODO Auto-generated constructor stub

}

Move::~Move()
{
  // TODO Auto-generated destructor stub
}

