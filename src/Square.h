/*
 * Square.h
 *
 *  Created on: 22.04.2016
 *      Author: nczempin
 */

#ifndef SQUARE_H_
#define SQUARE_H_
#include <string>

using namespace std;

class Square
{
public:
  Square();
  virtual ~Square();
  static void print_square(uint8_t square);
  static string mailbox_index_to_square(uint8_t x);
  static const int E1 = 4;
  static const int E8 = 64 - 4;

};

#endif /* SQUARE_H_ */
