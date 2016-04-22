/*
 * Square.cpp
 *
 *  Created on: 22.04.2016
 *      Author: admin
 */

#include "Square.h"
#include <iostream>

using namespace std;

Square::Square()
{
  // TODO Auto-generated constructor stub

}

Square::~Square()
{
  // TODO Auto-generated destructor stub
}

void Square::print_square(int x)
{
  string square = mailbox_index_to_square(x);
  cout << x << " = " << square << endl;

}

string Square::mailbox_index_to_square(int x)
{
  char column = 'a' + x % 8;
  string columnString(1, column);
  char row = '1' + x / 8;
  string rowString(1, row);
  string square = columnString + rowString;
  return square;
}
