/*
 * Move_container.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: nczempin
 */

#include "Move_container.h"
#include "Piece.h"
#include <array>
#include <iostream>

array<Move_container, 10> Move_container::container_pool;

Move_container::Move_container()
{
  Move m;
  for (size_t i = 0; i < SIZE; ++i) {
    container[i] = m;
  }
  index = 0;
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
    int8_t captured, bool en_passant_capture)
{
  if (moving == 0) {
    cerr << "moving == 0" << endl;
    throw 17;
  }
  if (index > this->SIZE || index < 0) {
    throw index;
  }
  Move &m = container[index];
  m.set_moving_piece(moving);
  m.set_from(from);
  m.set_to(to);
  m.set_captured(captured);
  m.set_en_passant_capture(en_passant_capture);
  m.set_en_passant_square(0);
  ++index;
  if (index > this->SIZE || index < 0) {
    throw index;
  }
  //TODO if index == capacity
}

void Move_container::reset()
{
  index = 0;
}

size_t Move_container::size()
{
  return index;
}

array<Move, Move_container::SIZE> Move_container::get_moves()
{
  return container; //TODO unsafe?
}
