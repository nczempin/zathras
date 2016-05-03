/*
 * Move_container.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: nczempin
 */

#include "Move_container.h"
#include "Piece.h"
#include <vector>
#include <iostream>

vector<Move_container> Move_container::container_pool(10);

Move_container::Move_container()
{

}

Move_container::~Move_container()
{
  // TODO Auto-generated destructor stub
}

Move_container& Move_container::get(size_t index)
{
  return container_pool[index];
}

void Move_container::add_move(int8_t moving, uint8_t from, uint8_t to,
    int8_t captured, bool en_passant)
{
  Move m(moving, from, to, captured, en_passant);
  container.push_back(m);
}

void Move_container::reset()
{
  container.clear();
}

size_t Move_container::size()
{
  return container.size();
}

vector<Move> Move_container::get_moves()
{
  return container; //TODO unsafe?
}
