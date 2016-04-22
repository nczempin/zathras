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

class Move {
public:
  Move(int piece, bb from, bb to, int captured);
  virtual ~Move();
  bb get_from()
  {
    return from;
  }
  bb get_to()
  {
    return to;
  }
  string to_string()
  {
    static const string pieces("- NBRQK");
    char p = pieces[moving];
    string moving_string = string(1, p);
    string retval = moving_string + Square::mailbox_index_to_square(from);
    retval += taken ? "x" : "-";
    retval += Square::mailbox_index_to_square(to);
    if (true || taken) {
      retval += " (" + string(1, pieces[taken]) + ")";
    }
    return retval;
  }
  int get_moving_piece() const
  {
    return moving;
  }

  void set_moving_piece(int moving)
  {
    this->moving = moving;
  }

  int get_taken_piece() const
  {
    return taken;
  }

  void set_taken_piece(int taken)
  {
    this->taken = taken;
  }

private:
  bb from;
  bb to;
  int moving;
  int taken;
};

#endif /* MOVE_H_ */
