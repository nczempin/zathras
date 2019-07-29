#include "Move_generator.h"

#include <bitset>
#include <utility>
#include <vector>

#include "Position.h"
#include "Square.h"
#include "Piece.h"
#include "Bitboard.h"


namespace Moves {
	bitboard_set Move_generator::pregenerate_rays(int8_t direction) {
		bitboard_set rays;
		bitset<64> bs[64];
		for (int i = 0; i < 64; ++i) {
			bitset<64> bbs = bs[i];
			for (int j = 0; j < 7; ++j) {
				int from = i + direction * j;
				square_t candidate = static_cast<square_t>(from + direction);
				if (candidate > 63 || candidate < 0) {
					continue;
				}
				/*int file_to = candidate % 8;
				int rank_to = candidate / 8;
				const int to = 7 - file_to + rank_to * 8;*/

				if (candidate >= 0 && candidate < 64) {
					set_square(bbs, candidate);
					if (from % 8 == 7 && candidate % 8 == 0) {
						clear_square(bbs, candidate);
					}
					if (from % 8 == 0 && candidate % 8 == 7) {
						clear_square(bbs, candidate);
					}
				}
				if (!bbs[candidate]) { // as soon as we hit an illegal target,
					break; // the ray ends
				}
			}

			unsigned long long as_int = bbs.to_ullong();
			rays[i] = as_int;
		}
		return rays;
	}

	bitboard_set Move_generator::pregenerate_hoppers(vector<int> hops) {
		bitboard_set hoppers_attacking; //
		bitset<64> attacking[64];
		for (int i = 0; i < 64; ++i) {
			for (int k : hops) {
				int candidate = k + i;
				if (candidate >= 0 && candidate < 64) {
					int file_from = i % 8;
					int file_to = candidate % 8;
					int rank_to = candidate / 8;
					set_square(file_to, rank_to, attacking[i]);
					if (file_from >= 6 && file_to <= 1) {
						clear_square(file_to, rank_to, attacking[i]);
					}
					if (file_from <= 1 && file_to >= 6) {
						clear_square(file_to, rank_to, attacking[i]);
					}
				}
			}
		}
		// collect them all
		for (int i = 0; i < 64; ++i) {
			hoppers_attacking[i] = attacking[i].to_ullong();
		}
		return hoppers_attacking;
	}
	bitboard_set Move_generator::pregenerate_knight_moves() {
		vector<int> nm{ 10, 17, 15, 6, -6, -15, -17, -10 };
		bitboard_set knight_moves = pregenerate_hoppers(nm);
		return knight_moves;
	}

	bitboard_set Move_generator::pregenerate_king_moves() {
		vector<int> km = { 1, 7, 8, 9, -1, -7, -8, -9 };
		bitboard_set king_moves = pregenerate_hoppers(km);
		return king_moves;
	}

	bitboard_set Move_generator::pregenerate_bishop_moves() {
		bitboard_set bishop_NE = pregenerate_rays(9);
		bitboard_set bishop_NW = pregenerate_rays(7);
		bitboard_set bishop_SE = pregenerate_rays(-9);
		bitboard_set bishop_SW = pregenerate_rays(-7);
		bitboard_set bishop_moves;
		for (int i = 0; i < 64; ++i) {
			bishop_moves[i] = bishop_NE[i] | bishop_NW[i] | bishop_SE[i] | bishop_SW[i];
		}
		return bishop_moves;
	}

	bitboard_set Move_generator::pregenerate_rook_moves() {
		bitboard_set rook_E = pregenerate_rays(1);
		bitboard_set rook_W = pregenerate_rays(-1);
		bitboard_set rook_S = pregenerate_rays(-8);
		bitboard_set rook_N = pregenerate_rays(8);
		bitboard_set rook_moves;
		for (int i = 0; i < 64; ++i) {
			rook_moves[i] = rook_E[i] | rook_W[i] | rook_S[i] | rook_N[i];
		}
		return rook_moves;
	}

	bitboard_set Move_generator::pregenerate_queen_moves() {
		bitboard_set bishop_moves = pregenerate_bishop_moves();
		bitboard_set rook_moves = pregenerate_rook_moves();
		bitboard_set queen_moves;
		for (int i = 0; i < 64; ++i) {
			queen_moves[i] = bishop_moves[i] | rook_moves[i];
		}
		return queen_moves;
	}

	void Move_generator::pregenerate_moves() {
	}

	bitboard_set Move_generator::pregen_pawn_caps(int direction) {
		bitset<64> bs[64];
		bitboard_set pawn_capture_moves;
		for (int i = 0; i < 64; ++i) {
			//TODO not the most elegant way to refactor/extract the function
			place_pawn_move(i, 9, direction, bs);
			place_pawn_move(i, 7, direction, bs);
		}
		for (int i = 0; i < 64; ++i) {
			unsigned long long as_int = bs[i].to_ullong();
			pawn_capture_moves[i] = as_int;
		}
		return pawn_capture_moves;
	}
	bitboard_set Move_generator::pregenerate_white_pawn_capture_moves() {
		bitboard_set white_pawn_capture_moves = pregen_pawn_caps(Piece::WHITE_PAWN);
		return white_pawn_capture_moves;
	}

	bitboard_set Move_generator::pregenerate_black_pawn_capture_moves() {
		bitboard_set black_pawn_capture_moves = pregen_pawn_caps(Piece::BLACK_PAWN);
		return black_pawn_capture_moves;

	}
	void Move_generator::place_pawn_move(int from, int steps, int direction, bitset<64> bs[64]) {
		int candidate = from + steps * direction;
		int file_to = candidate % 8;
		int rank_to = candidate / 8;

		if (candidate >= 0 && candidate < 64) {
			set_square(file_to, rank_to, bs[from]);
			if (from % 8 == 7 && candidate % 8 == 0) {
				clear_square(file_to, rank_to, bs[from]);
			}
			if (from % 8 == 0 && candidate % 8 == 7) {
				clear_square(file_to, rank_to, bs[from]);
			}
		}
	}

	bitboard_set Move_generator::pregen_pawn_nocaps(int start, int stop, int8_t direction) {
		bitset<64> bs[64];

		for (int i = start; i != 64 - start; i += direction) {
			int candidate = i + 8 * direction; // single step
			int file_to = candidate % 8;
			int rank_to = candidate / 8;
			set_square(file_to, rank_to, bs[i]);
		}
		bitboard_set pawn_no_capture_moves;
		for (int i = start; i != stop; i += direction) {
			int candidate = i + 16 * direction; // double step
			int file_to = candidate % 8;
			int rank_to = candidate / 8;
			set_square(file_to, rank_to, bs[i]);
		}
		for (int i = 0; i < 64; ++i) {
			unsigned long long as_int = bs[i].to_ullong();
			pawn_no_capture_moves[i] = as_int;
		}
		return pawn_no_capture_moves;

	}
	bitboard_set Move_generator::pregenerate_white_pawn_no_capture_moves() {
		bitboard_set white_pawn_no_capture_moves = pregen_pawn_nocaps(8, 16,
			Piece::WHITE_PAWN);
		return white_pawn_no_capture_moves;
	}
	bitboard_set Move_generator::pregenerate_black_pawn_no_capture_moves() {
		bitboard_set black_pawn_no_capture_moves = pregen_pawn_nocaps(56, 47,
			Piece::BLACK_PAWN);
		return black_pawn_no_capture_moves;
	}
	static bool on_same_file(const int& x, const int& y) {
		return (x % 8) == (y % 8);
	}
	static bool on_same_rank(const int& x, const int& y) {
		return (x / 8) == (y / 8);
	}

	static int determine_diagonal(const int& x, const int& y) {
		int smaller;
		int larger;
		if (y < x) {
			smaller = y;
			larger = x;
		}
		else {
			smaller = x;
			larger = y;
		}
		const int diff = larger - smaller;
		if (diff % 9 == 0) {
			return 1;
		}
		else if (diff % 7 == 0) {
			return -1;
		}
		return 0;
	}
	bb Move_generator::possibly_between_pre(const square_t& x, const square_t& y) {
		if (x == y) {
			return false;
		}
		//TODO this can be done much more elegantly and much more efficiently
		uint8_t smaller = uint8_t(x); // TODO proper cast
		uint8_t larger = uint8_t(y); // TODO proper cast
		if (y < x) {
			smaller = y;
			larger = x;
		}
		const uint8_t rank = smaller >> 3; // divide by 8
		const uint8_t file = smaller % 8;
		const uint8_t diff = larger - smaller;

		if (on_same_rank(x, y)) {
			//again with the elegance
			if (diff == 1) {
				return 0;
			}
			// evil magic numbers
			bitset<64> between = 0;
			for (uint8_t i = 1; i < diff; ++i) {

				set_square(file + i, rank, between);
			}
			const bb intersection = between.to_ullong();
			return intersection;
		}
		if (on_same_file(x, y)) {
			if (diff == 8) {
				return 0;
			}
			// evil magic numbers
			bitset<64> between = 0;
			uint8_t rank_to = rank;
			uint8_t ranks = diff >> 3; // divide by 8
			for (uint8_t i = 1; i < ranks; ++i) {
				++rank_to;
				set_square(file, rank_to, between);
			}
			const bb intersection = between.to_ullong();
			return intersection;
		}

		const int8_t diagonal = determine_diagonal(x, y);
		if (diff == 8 + diagonal) {
			return 0; // adjacent
		}
		bitset<64> between_l = 0;
		const uint8_t rank_larger = larger / 8;
		int i = 1;
		while (rank + i < rank_larger) {
			if (diagonal == 1 && rank + i * diagonal % 8 == 0) {
				break;
			}
			set_square(file + i * diagonal, rank + i, between_l);
			++i;
		}
		const bb intersection = between_l.to_ullong();
		return intersection;
	}
}