/*
 * Move_generator.cpp
 *
 *  Created on: 16.04.2016
 *      Author: nczempin
 */

#include "Move_generator.h"

#include <bitset>
#include <utility>
#include <vector>
#include "Position.h"

Move_generator::Move_generator() {
	// TODO Auto-generated constructor stub

}

Move_generator::~Move_generator() {
	// TODO Auto-generated destructor stub
}
pair<bitboard_set, bitboard_set> Move_generator::pregenerate_rays(
		int direction) {
	bitboard_set rays(64);
	bitset<64> bs[64];
	for (int i = 63; i >= 0; --i) {
		for (int j = 0; j < 7; ++j) {
			int from = i + direction * j;
			int candidate = from + direction;
			int file_to = candidate % 8;
			int rank_to = candidate / 8;
			int to = 7 - file_to + rank_to * 8;

			if (candidate >= 0 && candidate < 64) {
				Position::setSquare(bs[i], to);
				if (from % 8 == 7 && candidate % 8 == 0) {
					Position::clearSquare(bs[i], to);
				}
				if (from % 8 == 0 && candidate % 8 == 7) {
					Position::clearSquare(bs[i], to);
				}
			}
			if (!bs[i][to]) { // as soon as we hit an illegal target,
				break; // the ray ends
			}
		}
		unsigned long int as_int = bs[i].to_ulong();
		rays[i] = as_int;
	}
	pair<bitboard_set, bitboard_set> p(rays, rays);
	return p;
}

pair<bitboard_set, bitboard_set> Move_generator::pregenerate_hoppers(
		vector<int> hops) {
	bitboard_set hoppers_attacking(64);
	bitset<64> attacking[64];
	for (int i = 0; i < 64; ++i) {
		for (int k : hops) {
			int candidate = k + i;
			if (candidate >= 0 && candidate < 64) {
				int file_from = i % 8;
				int file_to = candidate % 8;
				int rank_to = candidate / 8;
				int to = 7 - file_to + rank_to * 8;
				Position::setSquare(attacking[i], to);
				if (file_from >= 6 && file_to <= 1) {
					Position::clearSquare(attacking[i], to);
				}
				if (file_from <= 1 && file_to >= 6) {
					Position::clearSquare(attacking[i], to);
				}
			}
		}
	}
	// collect them all
	for (int i = 0; i < 64; ++i) {
		hoppers_attacking[i] = attacking[i].to_ulong();
	}
	pair<bitboard_set, bitboard_set> p(hoppers_attacking, hoppers_attacking);
	return p;
}
pair<bitboard_set, bitboard_set> Move_generator::pregenerate_knight_moves() {
	vector<int> nm { 10, 17, 15, 6, -6, -15, -17, -10 };
	bitboard_set knight_moves = pregenerate_hoppers(nm).first;
	pair<bitboard_set, bitboard_set> p(knight_moves, knight_moves);
	return p;
}

pair<bitboard_set, bitboard_set> Move_generator::pregenerate_king_moves() {
	vector<int> km = { 1, 7, 8, 9, -1, -7, -8, -9 };
	bitboard_set king_moves = pregenerate_hoppers(km).first;
	pair<bitboard_set, bitboard_set> p(king_moves, king_moves);
	return p;
}

pair<bitboard_set, bitboard_set> Move_generator::pregenerate_bishop_moves() {
	pair<bitboard_set, bitboard_set> bishop_NE = pregenerate_rays(9);
	pair<bitboard_set, bitboard_set> bishop_NW = pregenerate_rays(7);
	pair<bitboard_set, bitboard_set> bishop_SE = pregenerate_rays(-9);
	pair<bitboard_set, bitboard_set> bishop_SW = pregenerate_rays(-7);
	bitboard_set bishop_moves(64);
	for (int i = 0; i < 64; ++i) {
		bishop_moves[i] = bishop_NE.first[i] | bishop_NW.first[i]
				| bishop_SE.first[i] | bishop_SW.first[i];
	}
	pair<bitboard_set, bitboard_set> p(bishop_moves, bishop_moves);
	return p;
}

pair<bitboard_set, bitboard_set> Move_generator::pregenerate_rook_moves() {
	pair<bitboard_set, bitboard_set> rook_E = pregenerate_rays(1);
	pair<bitboard_set, bitboard_set> rook_W = pregenerate_rays(-1);
	pair<bitboard_set, bitboard_set> rook_S = pregenerate_rays(-8);
	pair<bitboard_set, bitboard_set> rook_N = pregenerate_rays(8);
	bitboard_set rook_moves(64);
	for (int i = 0; i < 64; ++i) {
		rook_moves[i] = rook_E.first[i] | rook_W.first[i] | rook_S.first[i]
				| rook_N.first[i];
	}
	pair<bitboard_set, bitboard_set> p(rook_moves, rook_moves);
	return p;
}

pair<bitboard_set, bitboard_set> Move_generator::pregenerate_queen_moves() {
	pair<bitboard_set, bitboard_set> bishop_moves = pregenerate_bishop_moves();
	pair<bitboard_set, bitboard_set> rook_moves = pregenerate_rook_moves();
	bitboard_set queen_moves(64);
	for (int i = 0; i < 64; ++i) {
		queen_moves[i] = bishop_moves.first[i] | rook_moves.first[i];
	}
	pair<bitboard_set, bitboard_set> p(queen_moves, queen_moves);
	return p;
}

void Move_generator::pregenerate_moves() {
}

pair<bitboard_set, bitboard_set> Move_generator::pregen_pawn_caps(
		int direction) {
	bitset<64> bs[64];
	bitboard_set pawn_capture_moves(64);
	for (int i = 8; i < 56; ++i) {
		//TODO not the most elegant way to refactor/extract the function
		place_pawn_move(i, 9, direction, bs);
		place_pawn_move(i, 7, direction, bs);
	}
	for (int i = 0; i < 64; ++i) {
		unsigned long int as_int = bs[i].to_ulong();
		pawn_capture_moves[i] = as_int;
	}
	pair<bitboard_set, bitboard_set> p(pawn_capture_moves, pawn_capture_moves);
	return p;
}
pair<bitboard_set, bitboard_set> Move_generator::pregenerate_white_pawn_capture_moves() {
	pair<bitboard_set, bitboard_set> white_pawn_capture_moves =
			pregen_pawn_caps(1);
	return white_pawn_capture_moves;
}

pair<bitboard_set, bitboard_set> Move_generator::pregenerate_black_pawn_capture_moves() {
	pair<bitboard_set, bitboard_set> black_pawn_capture_moves =
			pregen_pawn_caps(-1);
	return black_pawn_capture_moves;

}
void Move_generator::place_pawn_move(int from, int steps, int direction,
		bitset<64> bs[64]) {
	int candidate = from + steps * direction;
	int file_to = candidate % 8;
	int rank_to = candidate / 8;
	int to = 7 - file_to + rank_to * 8;

	if (candidate >= 0 && candidate < 64) {
		Position::setSquare(bs[from], to);
		if (from % 8 == 7 && candidate % 8 == 0) {
			Position::clearSquare(bs[from], to);
		}
		if (from % 8 == 0 && candidate % 8 == 7) {
			Position::clearSquare(bs[from], to);
		}
	}
}

bitboard_set Move_generator::pregen_pawn_nocaps(int start, int stop,
		int direction) {
	bitset<64> bs[64];

	for (int i = start; i != 64 - start; i += direction) {
		int candidate = i + 8 * direction; // single step
		int file_to = candidate % 8;
		int rank_to = candidate / 8;
		int to = 7 - file_to + rank_to * 8;
		Position::setSquare(bs[i], to);
	}
	bitboard_set pawn_no_capture_moves(64);
	for (int i = start; i != stop; i += direction) {
		int candidate = i + 16 * direction; // double step
		int file_to = candidate % 8;
		int rank_to = candidate / 8;
		int to = 7 - file_to + rank_to * 8;
		Position::setSquare(bs[i], to);
	}
	for (int i = 0; i < 64; ++i) {
		unsigned long int as_int = bs[i].to_ulong();
		pawn_no_capture_moves[i] = as_int;
	}
	return pawn_no_capture_moves;

}
bitboard_set Move_generator::pregenerate_white_pawn_no_capture_moves() {
	bitboard_set white_pawn_no_capture_moves = pregen_pawn_nocaps(8, 16, 1);
	return white_pawn_no_capture_moves;
}
bitboard_set Move_generator::pregenerate_black_pawn_no_capture_moves() {
	bitboard_set black_pawn_no_capture_moves = pregen_pawn_nocaps(56, 47, -1);
	return black_pawn_no_capture_moves;
}
void Move_generator::generate_moves(Position position) {
	bitboard_set pieces = position.getPieceBitboards();
	bb white_pawns = pieces[1] & pieces[7];
	Position::visualize_bitboard(white_pawns, cout);
	Position::visit_bitboard(white_pawns, [this,position](int x) {
		string from = position.mailboxIndexToSquare(x);
		bb moves = white_pawn_no_capture_moves[x];
		Position::visit_bitboard(moves,[moves,position, from](int y) {
					string to = position.mailboxIndexToSquare(y);
					cout << from << to << endl;
				});
	});
}
