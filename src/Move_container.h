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
#include <array>

using namespace std;

class Move_container
{
public:
  Move_container();
  virtual ~Move_container();

  static const size_t SIZE = 128;

  static Move_container& get(size_t index);
  void add_move(int8_t moving, uint8_t from, uint8_t to, int8_t captured,
      bool en_passant_capture, int8_t promoted_to);
  void reset();
  size_t size();
  array<Move, SIZE> get_moves();
private:
  static array<Move_container, 10> container_pool;
  array<Move, SIZE> container;
  int index = 0;
};

#endif /* MOVE_CONTAINER_H_ */
