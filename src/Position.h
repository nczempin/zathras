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

using namespace std;

class Position
{
public:
  Position();
  virtual ~Position();
  static shared_ptr<Position> create_start_position();
  friend ostream& operator<<(ostream& stream, const Position& position);
  void print(ostream& stream) const;
  void pregenerate_moves();
private:
  bool white_to_move = true;
  uint_fast64_t pawns = 0x008469040000e700;
  uint_fast64_t knights = 0x4000000000000000;
  uint_fast64_t bishops = 0x0000100000100000;
  uint_fast64_t rooks = 0x0100000000000009;
  uint_fast64_t queens = 0x8000000010000000;
  uint_fast64_t kings = 0x0800000000000800;
  uint_fast64_t everything = 0xffffffffffffffff;
  uint_fast64_t top_row = 0xff00000000000000;
  uint_fast64_t white = 0x800000000010ef09;
  uint_fast64_t black = 0x4984790410000000;

  static string extract_row_string(uint_fast8_t row, string set);
  static void visualize_bitboard(uint_fast64_t bb, ostream& stream);
  //using hurz = function<void(int)>;
  void visit_bitboard(uint_fast64_t bb, function<void(int)>) const;
  static void visualize_mailbox_board(int board[64], ostream& stream);
  void visit_mailbox_board(int board[64], void (*f)(int)) const;
  vector<uint_fast64_t> pregenerate_hoppers(vector<int> jumps);
  vector<uint_fast64_t> pregenerate_rays(int direction);
  vector<uint_fast64_t> pregenerate_knight_moves();
  vector<uint_fast64_t> pregenerate_king_moves();
  vector<uint_fast64_t> pregenerate_bishop_moves();
  vector<uint_fast64_t> pregenerate_rook_moves();
  vector<uint_fast64_t> pregenerate_queen_moves();
  vector<uint_fast64_t> pregenerate_white_pawn_no_capture_moves();
  vector<uint_fast64_t> pregenerate_black_pawn_no_capture_moves();
  vector<uint_fast64_t> pregen_pawn_nocaps(int start, int stop, int direction);
  vector<uint_fast64_t> pregenerate_white_pawn_capture_moves();
  vector<uint_fast64_t> pregenerate_black_pawn_capture_moves();
  vector<uint_fast64_t> pregen_pawn_caps(int direction);
  void place_pawn_move(int i, int a, int direction, bitset<64> bs[64]);
};

#endif /* POSITION_H_ */
