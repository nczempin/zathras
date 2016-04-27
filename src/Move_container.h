/*
 * Move_container.h
 *
 *  Created on: Apr 28, 2016
 *      Author: nczempin
 */

#ifndef MOVE_CONTAINER_H_
#define MOVE_CONTAINER_H_

#include "Move.h"

#include <stddef.h>
#include <cstdint>
#include <vector>

using namespace std;

class Move_container
{
public:
  Move_container();
  virtual ~Move_container();
  static Move_container& get(size_t index);
  void add_move(int8_t moving, uint8_t from, uint8_t to, int8_t captured);
  void reset();
  vector<Move> get_moves();
private:
  static vector<Move_container> container_pool;
  vector<Move> container;
};

#endif /* MOVE_CONTAINER_H_ */
