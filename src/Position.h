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
  void generate_moves();
private:
  bool white_to_move = true;
  uint_fast64_t pawns = 0x00e7000004698400;
  uint_fast64_t knights = 0x0000000000000040;
  uint_fast64_t bishops = 0x0000100000100000;
  uint_fast64_t rooks = 0x0900000000000001;
  uint_fast64_t queens = 0x0000001000000080;
  uint_fast64_t kings = 0x0008000000000008;

  uint_fast64_t white = 0x09ef100000000080;
  uint_fast64_t black = 0x0000001004798449;

  static string extract_row_string(uint_fast8_t row, string set);
  static void visualize_bitboard(uint_fast64_t bb, ostream& stream);
  //using hurz = function<void(int)>;
  void visit_bitboard(uint_fast64_t bb, function<void(int)>) const;
  static void visualize_mailbox_board(int board[64], ostream& stream);
  void visit_mailbox_board(int board[64], void (*f)(int)) const;
};

#endif /* POSITION_H_ */
