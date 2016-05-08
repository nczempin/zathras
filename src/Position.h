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
#include <array>

using namespace std;

class Position
{
public:
  Position();
  virtual ~Position();

  void make_move(Move& move);
  void unmake_move(Move& move);

  friend ostream& operator<<(ostream& stream, const Position& position);
  void print(ostream& stream) const;
  static Position create_start_position();

  //TODO move; probably to Square class
  static void set_square(bitset<64>& bs, int to);
  static void clear_square(bitset<64>& bs, int to);
  static void set_square(bb& bs, int to);
  static bool is_set_square(bb bs, int to);
  static void clear_square(bb& bs, int to);
  static void set_bit(bb& bs, int to);
  static void clear_bit(bb& bs, int to);
  static int set_square(int file_to, int rank_to, bitset<64>& bbs);
  static int clear_square(int file_to, int rank_to, bitset<64>& bbs);
  static int set_square(int file_to, int rank_to, bb& bbs);
  static int clear_square(int file_to, int rank_to, bb& bbs);

  static void visualize_bitboard(bb my_bb, ostream& stream);
  static void visit_bitboard(const bb my_bb, const square_visitor);
//  static void visit_bitboard2(const bb my_bb, const square_visitor);
  static void visualize_mailbox_board(int board[64], ostream& stream);
  static void visit_mailbox_board(int board[64], void (*visitor)(int)); // TODO convert to c++11

  static Position create_position(const string& fen);
  static uint8_t extract_square(const bb my_bb);
  static uint8_t extract_and_remove_square(bb& my_bb);
  //static void print_square(int x);
  //TODO a separate Bitboard (helper) class is probably best
  static const bb BB_FULL_BOARD = 0xffffffffffffffff;
  static const bb BB_RANK8 = 0xff00000000000000;
  static const bb BB_RANK3 = 0x0000000000ff0000;
  static const bb BB_RANK4 = 0x00000000ff000000;
  static const bb BB_RANK5 = 0x000000ff00000000;
  static const bb BB_RANK6 = 0x0000ff0000000000;
  static const bb BB_RANK3N6 = BB_RANK3 | BB_RANK6;
  array<bb, 9> getPieceBitboards() const;

  bool is_white_to_move() const
  {
    return white_to_move;
  }
  bool white_to_move = true; //TODO public for now
  bb en_passant_square = 0x00;
  bool castling[4];

  //public for now
  bb pawns = 0x00;
  bb knights = 0x00;
  bb bishops = 0x00;
  bb rooks = 0x00;
  bb queens = 0x00;
  bb kings = 0x00;
  bb white = 0x00;
  bb black = 0x00;
private:

  static string extract_row_string(uint_fast8_t row, string set);
  static void display_all_moves(const bitboard_set& moves);
  void update_bits(unsigned long int& colour, unsigned long int& piece,
      uint8_t from, uint8_t to);
  void save_en_passant_square(Move& move);
  void restore_en_passant_square(Move& move);
};

#endif /* POSITION_H_ */
