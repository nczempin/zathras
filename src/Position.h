/*
 * Position.h
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#ifndef POSITION_H_
#define POSITION_H_

#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Position
{
public:
  Position();
  virtual ~Position();
  static shared_ptr<Position> create_start_position();
  friend ostream& operator<<(ostream& stream, const Position& position);
  void print(ostream& stream) const;
private:
  string extract_row_string(uint_fast8_t row, string set) const;
  void visualize_bitboard(uint_fast64_t bb, ostream& stream) const;
  //using hurz = function<void(int)>;
  void visit_bitboard(uint_fast64_t bb, function<void(int)>) const;
  void visualize_mailbox_board(int board[64], ostream& stream) const;
  void visit_mailbox_board(int board[64], void (*f)(int)) const;
};

#endif /* POSITION_H_ */
