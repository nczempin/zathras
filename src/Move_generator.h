/*
 * Move_generator.h
 *
 *  Created on: 16.04.2016
 *      Author: nczempin
 */

#ifndef MOVE_GENERATOR_H_
#define MOVE_GENERATOR_H_

#include "Position.h"

class Move_generator {
public:
	Move_generator();
	virtual ~Move_generator();
	static void pregenerate_moves();
	void generate_moves(Position position);
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
	void print_moves(bb ppp, bitboard_set mmm, Position position);

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

};

#endif /* MOVE_GENERATOR_H_ */
