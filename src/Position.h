/*
 * Position.h
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#ifndef POSITION_H_
#define POSITION_H_

#include <bitset>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "typedefs.h"
#include "Move.h"
using namespace std;

class Position {
public:
  Position();
  virtual ~Position();

  void make_move(Move move);
  void unmake_move(Move move);

  friend ostream& operator<<(ostream& stream, const Position& position);
  void print(ostream& stream) const;
  static void set_square(bitset<64>& bs, int to);
  static void clear_square(bitset<64>& bs, int to);
  static void set_square(bb& bs, int to);
  static void clear_square(bb& bs, int to);

  static int set_square(int file_to, int rank_to, bitset<64>& bbs);
  static int clear_square(int file_to, int rank_to, bitset<64>& bbs);
  static int set_square(int file_to, int rank_to, bb& bbs);
  static int clear_square(int file_to, int rank_to, bb& bbs);
  static void visualize_bitboard(bb my_bb, ostream& stream);
  static void visit_bitboard(bb my_bb, square_visitor);
  static void visualize_mailbox_board(int board[64], ostream& stream);
  static void visit_mailbox_board(int board[64], void (*visitor)(int)); // TODO convert to c++11
  static void print_square(int x);
  static string mailboxIndexToSquare(int x);
  //TODO a separate Bitboard (helper) class is probably best
  static const bb BB_FULL_BOARD = 0xffffffffffffffff;
  static const bb BB_RANK8 = 0xff00000000000000;
  static const bb BB_RANK3 = 0x0000000000ff0000;
  static const bb BB_RANK4 = 0x00000000ff000000;
  static const bb BB_RANK5 = 0x000000ff00000000;
  static const bb BB_RANK6 = 0x0000ff0000000000;
  static const bb BB_RANK3N6 = BB_RANK3 | BB_RANK6;
  static Position create_start_position();
  bitboard_set getPieceBitboards();
  int determine_piece(int pc);
private:
  bool white_to_move = true;
  bb pawns = 0;
  bb knights = 0;
  bb bishops = 0;
  bb rooks = 0;
  bb queens = 0;
  bb kings = 0;
  bb white = 0;
  bb black = 0;

  static string extract_row_string(uint_fast8_t row, string set);
  static void display_all_moves(const bitboard_set& moves);
  static Position create_position(const string& fen);
};

#endif /* POSITION_H_ */
