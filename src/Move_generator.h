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
	void generate_moves();
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

};

#endif /* MOVE_GENERATOR_H_ */
