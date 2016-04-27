/*
 * Move_container.cpp
 *
 *  Created on: Apr 28, 2016
 *      Author: nczempin
 */

#include "Move_container.h"
#include <vector>

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
    int8_t captured)
{
  Move m(moving, from, to, captured);
  container.push_back(m);
}

void Move_container::reset()
{
  container.clear();
}

vector<Move> Move_container::get_moves()
{
  return container; //TODO unsafe?
}
