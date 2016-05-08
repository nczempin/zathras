/*
 * Move_generator.h
 *
 *  Created on: 16.04.2016
 *      Author: nczempin
 */

#ifndef MOVE_GENERATOR_H_
#define MOVE_GENERATOR_H_

#include "Position.h"
#include "Move.h"
#include "Move_container.h"

class Move_generator
{
public:
  Move_generator();
  virtual ~Move_generator();
  static void pregenerate_moves();
  Move_container generate_moves(shared_ptr<Position> p, size_t depth);
  bool is_in_check(bool side);
private:
  shared_ptr<Position> p;
  static bitboard_set pregenerate_rays(int8_t direction);
  static bitboard_set pregenerate_knight_moves();
  static bitboard_set pregenerate_king_moves();
  static bitboard_set pregenerate_bishop_moves();
  static bitboard_set pregenerate_rook_moves();
  static bitboard_set pregenerate_queen_moves();
  static bitboard_set pregenerate_white_pawn_no_capture_moves();
  static bitboard_set pregenerate_black_pawn_no_capture_moves();
  static bitboard_set pregen_pawn_nocaps(int start, int stop, int8_t direction);
  static bitboard_set pregenerate_white_pawn_capture_moves();
  static bitboard_set pregenerate_black_pawn_capture_moves();
  static bitboard_set pregen_pawn_caps(int direction);
  static bitboard_set pregenerate_hoppers(vector<int> jumps);
  static void place_pawn_move(int from, int steps, int direction,
      bitset<64> bs[64]);
  void print_moves_raw(bb ppp, bitboard_set mmm, Position position);

  static bitboard_set knight_moves;
  static bitboard_set king_moves;
  static bitboard_set bishop_moves;
  static bitboard_set rook_moves;
  static bitboard_set queen_moves;
  static bitboard_set white_pawn_no_capture_moves;
  static bitboard_set black_pawn_no_capture_moves;
  static bitboard_set white_pawn_capture_moves;
  static bitboard_set black_pawn_capture_moves;

  //array<bb, 9> pieces;

  void visit_moves_raw(const bb sub_position, const bitboard_set all_moves,
      move_visitor f, int moving);
  void visit_capture_moves(const bb sub_position, const bitboard_set all_moves,
      const move_visitor f, const bb other_colour, const int8_t moving);
  void visit_non_capture_moves(const bb sub_position,
      const bitboard_set all_moves, move_visitor f, bb other_colour,
      int8_t moving);
  void visit_non_capture_ray_moves(const bb sub_position,
      const bitboard_set all_moves, const move_visitor f, const bb occupied,
      const int8_t moving);
  void visit_capture_ray_moves(const bb sub_position,
      const bitboard_set all_moves, move_visitor f, bb occupied,
      bb other_colour, int moving);
  void visit_pawn_nocaps(const bb sub_position, const bitboard_set all_moves,
      move_visitor f, bb occupied, int8_t moving, bool white_to_move);
  static bb filter_occupied_squares(bool white_to_move, bb occupied,
      const bitboard_set& all_moves, int x);
  static bool is_anything_between(int x, int y, bb occupied);
  static int set_square(int file_to, int rank_to, bitset<64>& bbs);
  static int clear_square(int file_to, int rank_to, bitset<64>& bbs);
  void visit_moves(move_visitor count_moves);
  int8_t find_captured_piece(uint8_t square);
  void generate_castling(const move_visitor& f, bool white);
  bool is_attacked(uint8_t square);
  void attempt_castle(const move_visitor f, const int8_t piece,
      const uint8_t king_square, const int8_t direction);
  void f(Move_container& moves, const int8_t moving, const uint8_t from,
      const uint8_t to, const int8_t captured);
  bool is_check(const bb movers, const bitboard_set& all_moves,
      const uint8_t king_pos);
  bool is_check_from_slider(const bitboard_set& sliding_moves,
      const uint8_t king_pos, const bb slider, const bb& occupied);
};

#endif /* MOVE_GENERATOR_H_ */
