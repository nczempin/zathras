/*
 * Square.h
 *
 *  Created on: 22.04.2016
 *      Author: admin
 */

#ifndef SQUARE_H_
#define SQUARE_H_
#include <string>

using namespace std;

class Square {
public:
  Square();
  virtual ~Square();
  static void print_square(uint8_t square);
  static string mailbox_index_to_square(int square);

};

#endif /* SQUARE_H_ */
