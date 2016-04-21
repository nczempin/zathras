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

class Move_generator {
public:
  Move_generator();
  virtual ~Move_generator();
  static void pregenerate_moves();
  vector<Move> generate_moves(Position position);
private:
  static pair<bitboard_set, bitboard_set> pregenerate_rays(int direction);
  static pair<bitboard_set, bitboard_set> pregenerate_knight_moves();
  static pair<bitboard_set, bitboard_set> pregenerate_king_moves();
  static pair<bitboard_set, bitboard_set> pregenerate_bishop_moves();
  static pair<bitboard_set, bitboard_set> pregenerate_rook_moves();
  static pair<bitboard_set, bitboard_set> pregenerate_queen_moves();
  static bitboard_set pregenerate_white_pawn_no_capture_moves();
  static bitboard_set pregenerate_black_pawn_no_capture_moves();
  static bitboard_set pregen_pawn_nocaps(int start, int stop, int direction);
  static pair<bitboard_set, bitboard_set> pregenerate_white_pawn_capture_moves();
  static pair<bitboard_set, bitboard_set> pregenerate_black_pawn_capture_moves();
  static pair<bitboard_set, bitboard_set> pregen_pawn_caps(int direction);
  static pair<bitboard_set, bitboard_set> pregenerate_hoppers(
      vector<int> jumps);
  static void place_pawn_move(int from, int steps, int direction,
      bitset<64> bs[64]);
  void print_moves_raw(bb ppp, bitboard_set mmm, Position position);

  pair<bitboard_set, bitboard_set> knight_moves = pregenerate_knight_moves();
  pair<bitboard_set, bitboard_set> king_moves = pregenerate_king_moves();
  pair<bitboard_set, bitboard_set> bishop_moves = pregenerate_bishop_moves();
  pair<bitboard_set, bitboard_set> rook_moves = pregenerate_rook_moves();
  pair<bitboard_set, bitboard_set> queen_moves = pregenerate_queen_moves();
  bitboard_set white_pawn_no_capture_moves =
      pregenerate_white_pawn_no_capture_moves();
  bitboard_set black_pawn_no_capture_moves =
      pregenerate_black_pawn_no_capture_moves();
  pair<bitboard_set, bitboard_set> white_pawn_capture_moves =
      pregenerate_white_pawn_capture_moves();
  pair<bitboard_set, bitboard_set> black_pawn_capture_moves =
      pregenerate_black_pawn_capture_moves();
  static const int PAWN = 1;
  static const int KNIGHT = 2;
  static const int BISHOP = 3;
  static const int ROOK = 4;
  static const int QUEEN = 5;
  static const int KING = 6;
  static const int WHITE_PAWN = 1;
  static const int WHITE_KNIGHT = 2;
  static const int WHITE_BISHOP = 3;
  static const int WHITE_ROOK = 4;
  static const int WHITE_QUEEN = 5;
  static const int WHITE_KING = 6;
  static const int WHITE = 7;
  void visit_moves_raw(const bb sub_position, const bitboard_set all_moves,
      move_visitor f, int moving);
  void visit_capture_moves(const bb sub_position, const bitboard_set all_moves,
      move_visitor f, bb other_colour, int moving);
  void visit_non_capture_moves(const bb sub_position,
      const bitboard_set all_moves, move_visitor f, bb other_colour,
      int moving);
  void visit_non_capture_ray_moves(const bb sub_position,
      const bitboard_set all_moves, move_visitor f, bb occupied,
      bool white_to_move);

  void visit_capture_ray_moves(const bb sub_position,
      const bitboard_set all_moves, move_visitor f, bb occupied,
      bb other_colour, bool white_to_move);
  void visit_pawn_nocaps(const bb sub_position, const bitboard_set all_moves,
      move_visitor f, bb occupied, bool white_to_move);
  static bb filter_occupied_squares(bool white_to_move, bb occupied,
      const bitboard_set& all_moves, int x);
  static bool is_anything_between(int x, int y, bb occupied);
  static int set_square(int file_to, int rank_to, bitset<64>& bbs);
  static int clear_square(int file_to, int rank_to, bitset<64>& bbs);
  void visit_moves(const bitboard_set& pieces, move_visitor count_moves);
};

#endif /* MOVE_GENERATOR_H_ */
