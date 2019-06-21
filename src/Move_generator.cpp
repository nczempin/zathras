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
#include "Square.h"
#include "Piece.h"
#include "Bitboard.h"
#include "Info.h"

Move_generator::Move_generator() {
}

Move_generator::~Move_generator() {
	// TODO Auto-generated destructor stub
}
bool Move_generator::init_between() {
	uint8_t x, y;
	for (uint8_t i = 0; i < 64; ++i) {
		for (uint8_t j = 0; j < 64; ++j) {
			bb poss = possibly_between_pre(i, j);
			y = i;
			x = j;
			size_t index = Position::calc_index(x, y);
			Position::between[index] = poss;
		}
	}

	return true; //TODO why do I need a return value at all?
}
bool Move_generator::between_initialized = init_between(); // TODO workaround for missing static initializer block

// TODO move these outside
void Move_generator::set_square(const int& file_to, const int& rank_to,
	bitset<64> & bbs) {
	const unsigned int to_twisted = 7 - file_to + rank_to * 8;
	Position::set_square(bbs, to_twisted);
}

int Move_generator::clear_square(int file_to, int rank_to, bitset<64> & bbs) {
	int to_twisted = 7 - file_to + rank_to * 8;
	int to = file_to + rank_to * 8;
	Position::clear_square(bbs, to_twisted);
	return to;
}

bitboard_set Move_generator::pregenerate_rays(int8_t direction) {
	bitboard_set rays;
	bitset<64> bs[64];
	for (int i = 63; i >= 0; --i) {
		bitset<64> bbs = bs[i];
		for (int j = 0; j < 7; ++j) {
			int from = i + direction * j;
			int candidate = from + direction;
			if (candidate > 63 || candidate < 0) {
				continue;
			}
			int file_to = candidate % 8;
			int rank_to = candidate / 8;
			const int to = 7 - file_to + rank_to * 8;

			if (candidate >= 0 && candidate < 64) {
				set_square(file_to, rank_to, bbs);
				if (from % 8 == 7 && candidate % 8 == 0) {
					clear_square(file_to, rank_to, bbs);
				}
				if (from % 8 == 0 && candidate % 8 == 7) {
					clear_square(file_to, rank_to, bbs);
				}
			}
			if (!bbs[to]) { // as soon as we hit an illegal target,
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
		bishop_moves[i] = bishop_NE[i] | bishop_NW[i] | bishop_SE[i]
			| bishop_SW[i];
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
void Move_generator::place_pawn_move(int from, int steps, int direction,
	bitset<64> bs[64]) {
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

bitboard_set Move_generator::pregen_pawn_nocaps(int start, int stop,
	int8_t direction) {
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

//void Move_generator::print_moves_raw(const bb sub_position,
//	const bitboard_set all_moves, const Position position) {
//	visit_moves_raw(sub_position, all_moves,
//		[](int8_t pc, uint8_t x, uint8_t y, int8_t cpt, int8_t promoted_to) {
//			string from = Square::mailbox_index_to_square(x);
//			string to = Square::mailbox_index_to_square(y);
//			cout << from << to << endl;
//		}, position.is_white_to_move());
//}

int8_t Move_generator::find_captured_piece(uint8_t square, int8_t moving) {
	//cout << "looking for captured piece at: " << y << endl;
	int8_t captured = 0;
	if (Position::is_set_square(p->pawns, square)) {
		captured = Piece::PAWN;
	}
	else if (Position::is_set_square(p->knights, square)) {
		captured = Piece::KNIGHT;
	}
	else if (Position::is_set_square(p->bishops, square)) {
		captured = Piece::BISHOP;
	}
	else if (Position::is_set_square(p->rooks, square)) {
		captured = Piece::ROOK;
	}
	else if (Position::is_set_square(p->queens, square)) {
		captured = Piece::QUEEN;
	}
	else if (Position::is_set_square(p->kings, square)) {
		captured = Piece::KING; // should this even happen? exception?
	}
	if (Position::is_set_square(p->black, square)) { //black
		captured = -captured;
	}
	else {
		if (!Position::is_set_square(p->white, square)) {
			if (p->en_passant_square == 0) {
				p->debugPosition();
				throw 68;
			}
		}
	}
	if (p->en_passant_square != 0) {
		Bitboard::visit_bitboard(p->en_passant_square,
			[&captured, &square, &moving](uint8_t en_passant_capture_square) {
				if (square == en_passant_capture_square) {
					if (moving == Piece::BLACK_PAWN) {
						captured = Piece::WHITE_PAWN; //
					}
					if (moving == Piece::WHITE_PAWN && en_passant_capture_square > 31) { // in black half, must be black pawn
						captured = Piece::BLACK_PAWN;
					}
				}
			});
	}

	//cout << "found: " << captured << endl;
	if ((moving < 0 && captured < 0) || (moving > 0 && captured > 0)) {
		p->debugPosition();
		throw 27;
	}

	return captured;
}

void Move_generator::visit_capture_moves(const bb& sub_position,
	const bitboard_set& all_moves, const move_visitor& f,
	const bb& other_colour, const int8_t& moving) {
	bb position = sub_position;
	while (position != 0) {
		const uint8_t from = Bitboard::extract_and_remove_square(position);
		if (from == 255) {
			break; //TODO
		}
		const bb raw_moves = all_moves[from];
		bb moves = raw_moves & other_colour;
		//TODO move this to a separate visit_pawn_caps method
		while (moves != 0x00) {
			uint8_t to = Bitboard::extract_and_remove_square(moves);
			int8_t captured = find_captured_piece(to, moving);
			f(moving, from, to, captured, 0);
		}
	}
}

void Move_generator::visit_pawn_caps(const bb& sub_position,
	const bitboard_set& all_moves, const move_visitor& f,
	const bb& other_colour, const int8_t& moving) {
	bb position = sub_position;
	while (position != 0) {
		const uint8_t from = Bitboard::extract_and_remove_square(position);
		const bb raw_moves = all_moves[from];
		bb moves = raw_moves & other_colour;

		if ((moving == Piece::BLACK_PAWN
			&& (p->en_passant_square & Bitboard::BB_RANK3))
			|| (moving == Piece::WHITE_PAWN
				&& (p->en_passant_square & Bitboard::BB_RANK6))) {
			moves = raw_moves & (other_colour | p->en_passant_square);
			//TODO: for e.p. capture we know the captured piece is opposing pawn
		}
		while (moves != 0x00) {
			uint8_t to = Bitboard::extract_and_remove_square(moves);
			int8_t captured = find_captured_piece(to, moving);
			if (captured != 0) {
				if (to >= 56) { // promoting white pawn
					f(moving, from, to, captured, Piece::WHITE_QUEEN);
					//TODO switch subpromotions on/off here
					f(moving, from, to, captured, Piece::WHITE_ROOK);
					f(moving, from, to, captured, Piece::WHITE_BISHOP);
					f(moving, from, to, captured, Piece::WHITE_KNIGHT);
					// end switch subpromotions on/off

				}
				else if (to <= 7) { // promoting black pawn
					f(moving, from, to, captured, Piece::BLACK_QUEEN);
					//TODO switch subpromotions on/off here
					f(moving, from, to, captured, Piece::BLACK_ROOK);
					f(moving, from, to, captured, Piece::BLACK_BISHOP);
					f(moving, from, to, captured, Piece::BLACK_KNIGHT);
					// end switch subpromotions on/off

				}
				else {
					f(moving, from, to, captured, 0);
				}
			}
		}
	}
}
void Move_generator::visit_non_capture_moves(const bb& sub_position,
	const bitboard_set& all_moves, const move_visitor& f,
	const bb& other_colour, const int8_t& moving) {
	bb position = sub_position;
	while (position != 0) {
		const uint8_t from = Bitboard::extract_and_remove_square(position);
		if (from == 255) { //TODO
			break;
		}
		const bb raw_moves = all_moves[from];
		bb moves = raw_moves & ~other_colour;
		//TODO move this to a separate visit_pawn_caps method
		while (moves != 0x00) {
			uint8_t to = Bitboard::extract_and_remove_square(moves);
			f(moving, from, to, 0, 0);
		}
	}
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

bb Move_generator::possibly_between_pre(const uint8_t& x, const uint8_t& y) {
	if (x == y) {
		return false;
	}
	//TODO this can be done much more elegantly and much more efficiently
	uint8_t smaller = x;
	uint8_t larger = y;
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
void Move_generator::visit_non_capture_ray_moves(const bb& sub_position,
	const bitboard_set& all_moves, const move_visitor& f,
	const bb& occupied, const int8_t& moving) {
	bb position = sub_position;
	while (position != 0) {
		const uint8_t from = Bitboard::extract_and_remove_square(position);
		const bb raw_moves = all_moves[from];
		bb moves = raw_moves & ~occupied;
		while (moves != 0x00) {
			uint8_t to = Bitboard::extract_and_remove_square(moves);
			bool b = Position::is_anything_between(from, to, occupied);
			if (!b) {
				f(moving, from, to, 0, 0);
			}
		}
	}
}
void Move_generator::visit_capture_ray_moves(const bb& sub_position,
	const bitboard_set& all_moves, const move_visitor& f,
	const bb& occupied, const bb& other_colour, const int8_t& moving) {
	bb position = sub_position;
	while (position != 0) {
		const uint8_t from = Bitboard::extract_and_remove_square(position);
		const bb raw_moves = all_moves[from];
		bb moves = raw_moves & other_colour;
		while (moves != 0x00) {
			uint8_t to = Bitboard::extract_and_remove_square(moves);
			bool b = Position::is_anything_between(from, to, occupied);
			if (!b) {
				int8_t captured = find_captured_piece(to, moving);
				f(moving, from, to, captured, 0);
			}
		}
	}

}
void Move_generator::visit_moves_raw(const bb sub_position,
	const bitboard_set all_moves, move_visitor f, int8_t moving) {

	Bitboard::visit_bitboard(sub_position,
		[&all_moves, &f, &moving](uint8_t x) {
			Bitboard::visit_bitboard(all_moves[x], [&x, &f, &moving](uint8_t y) {
				int8_t captured = -98;
				f(moving, x, y, captured, 0); //TODO get the captured piece from somewhere
				});

		});
}

bb Move_generator::filter_occupied_squares(bool white_to_move, bb occupied,
	const bitboard_set& all_moves, int x) {
	//TODO simplify the distinction between white's move and black's move
	const bb jump_over =
		white_to_move ? Bitboard::BB_RANK3 : Bitboard::BB_RANK6;
	bb jump_over_occupied = jump_over & occupied;
	bb jump_over_occupants_shifted =
		white_to_move ? jump_over_occupied << 8 : jump_over_occupied >> 8;
	bb all_moves_from_here = all_moves[x];
	const bb jump_onto =
		white_to_move ? Bitboard::BB_RANK4 : Bitboard::BB_RANK5;
	bb all_moves_to_fifth =
		(x >= 48 && x < 56) ? all_moves_from_here & jump_onto : 0;
	bb all_moves_to_fourth =
		(x >= 8 && x < 16) ? all_moves_from_here & jump_onto : 0;
	bb all_moves_to_jump =
		white_to_move ? all_moves_to_fourth : all_moves_to_fifth;
	bb filter_out = all_moves_to_jump & jump_over_occupants_shifted;
	bb moves = (all_moves_from_here & ~occupied) & ~filter_out;
	return moves;
}

void Move_generator::visit_pawn_nocaps(const bb& sub_position,
	const bitboard_set& all_moves, const move_visitor& f,
	const bb& occupied, const int8_t& moving, const bool& white_to_move) {
	bb position = sub_position;
	while (position != 0) {
		const uint8_t from = Bitboard::extract_and_remove_square(position);
		bb moves = filter_occupied_squares(white_to_move, occupied, all_moves,
			from);
		while (moves != 0x00) {
			uint8_t to = Bitboard::extract_and_remove_square(moves);
			if (to >= 56) { // promoting white pawn
				f(moving, from, to, 0, Piece::WHITE_QUEEN);
				//TODO switch subpromotions on/off here
				f(moving, from, to, 0, Piece::WHITE_ROOK);
				f(moving, from, to, 0, Piece::WHITE_BISHOP);
				f(moving, from, to, 0, Piece::WHITE_KNIGHT);
				// end switch subpromotions on/off

			}
			else if (to <= 7) { // promoting black pawn
				f(moving, from, to, 0, Piece::BLACK_QUEEN);
				//TODO switch subpromotions on/off here
				f(moving, from, to, 0, Piece::BLACK_ROOK);
				f(moving, from, to, 0, Piece::BLACK_BISHOP);
				f(moving, from, to, 0, Piece::BLACK_KNIGHT);
				// end switch subpromotions on/off

			}
			else {
				f(moving, from, to, 0, 0);
			}
		}
	}
}

void Move_generator::attempt_castle(const move_visitor f, const int8_t piece,
	const uint8_t king_square, const int8_t direction) {
	//1. check if squares between king and rook are free.
	uint8_t next_square = king_square + direction;
	uint8_t target_square = king_square + direction * 2;

	if (p->is_set_square(p->white | p->black, next_square)) {
		return;
	}
	if (p->is_set_square(p->white | p->black, target_square)) {
		return;
	}
	if (direction == -1) { // queen side
		if (p->is_set_square(p->white | p->black, target_square - 1)) { // b1/b8
			return;
		}

	}

	//2. check if squares between king and rook are attacked.
	p->white_to_move = !p->white_to_move;

	bool attacked = is_attacked(king_square);
	if (attacked) {
		p->white_to_move = !p->white_to_move;
		return;
	}
	bool next = is_attacked(next_square);
	if (next) {
		p->white_to_move = !p->white_to_move;
		return;
	}
	bool target = is_attacked(target_square);
	if (target) {
		p->white_to_move = !p->white_to_move;
		return;
	}
	p->white_to_move = !p->white_to_move;

	f(piece, king_square, king_square + direction * 2, 0, 0);
}

void Move_generator::generate_castling(const move_visitor& f,
	bool white_to_move) {
	int8_t piece = Piece::WHITE_KING;
	uint8_t king_square = Square::E1;
	if (!white_to_move) {
		piece = Piece::BLACK_KING;
		king_square = Square::E8;
	}
	static int8_t king_jump_direction[] = { 1, -1, 1, -1 };
	static bool colour[] = { true, true, false, false };
	for (size_t i = 0; i < 4; ++i) {
		if (white_to_move == colour[i] && p->castling[i]) {
			attempt_castle(f, piece, king_square, king_jump_direction[i]);
		}
	}
}
void Move_generator::f(Move_container& moves, const int8_t moving,
	const uint8_t from, const uint8_t to, const int8_t captured,
	const int8_t promoted_to) {
	bool en_passant_capture = will_be_en_passant(to, moving);

	moves.add_move(moving, from, to, captured, en_passant_capture, promoted_to);
}

bool Move_generator::will_be_en_passant(uint8_t to, int8_t moving) {
	bool en_passant_capture = false;
	if (Position::is_set_square(p->en_passant_square, to)) {
		if ((moving == Piece::WHITE_PAWN && to > 31)
			|| (moving == Piece::BLACK_PAWN && to < 31)) {
			en_passant_capture = true;
		}
	}
	return en_passant_capture;
}

void Move_generator::add_non_capture_ray_moves(Move_container& moves,
	const int8_t piece, bb position, const bitboard_set& pieceMoves,
	const bb& occupied) {
	while (position != 0) {
		const uint8_t from = Bitboard::extract_and_remove_square(position);
		const bb raw_moves = pieceMoves[from]; //TODO potential bug flagged here: "C28020: The expression '0<=_Param_(1)&&_Param_(1)<=64-1' is not true at this call."
		bb moves_bb = raw_moves & ~occupied;
		while (moves_bb != 0x00) {
			const uint8_t& to = Bitboard::extract_and_remove_square(moves_bb);
			const bool& b = Position::is_anything_between(from, to, occupied);
			if (!b) {
				moves.add_move(piece, from, to, 0, false, 0);
			}
		}
	}
}
Move_container Move_generator::generate_legal_captures(Position position, int depth) {
	//TODO this is all terrible
	Move_container pseudolegal_moves = generate_pseudolegal_captures(position, depth);
	Move_container legal_moves;
	auto moves = pseudolegal_moves.get_moves();
	for (int i = 0; i < pseudolegal_moves.size(); ++i) {
		Move move = moves[i];
		//Position pos2 = position; //copied //TODO inefficient
		Move_state ms;
		position.make_move(move, ms);
		++Info::nodes;
		if (!position.is_in_check(!position.is_white_to_move())) {
			legal_moves.add_move(move);// moving, from, to, captured, en_passant_capture, promoted_to);
		}
		position.unmake_move(move, ms);

	}
	return legal_moves;
}
Move_container Move_generator::generate_legal_moves(Position position, size_t depth) {
	//TODO this is all terrible
	Move_container pseudolegal_moves = generate_pseudolegal_moves(position, depth);
	Move_container legal_moves;
	auto moves = pseudolegal_moves.get_moves();
	for (int i = 0; i < pseudolegal_moves.size(); ++i){
		Move move = moves[i];
		//Position pos2 = position; //copied //TODO inefficient
		Move_state ms;
		position.make_move(move, ms);
		++Info::nodes;

		if (!position.is_in_check(!position.is_white_to_move())) {
			legal_moves.add_move(move);// moving, from, to, captured, en_passant_capture, promoted_to);
		}
		position.unmake_move(move, ms);
	}
	return legal_moves;
}

Move_container Move_generator::generate_pseudolegal_moves(Position position, size_t depth) {
	p = &position;

	Move_container& moves = Move_container::get(depth);
	//moves.reserve(35);
	moves.reset();
	const move_visitor& f =
		[&moves, this](const int8_t& moving, const uint8_t& from, const uint8_t& to, const int8_t& captured, const int8_t& promoted_to) {

		bool en_passant_capture = will_be_en_passant(to, moving);
		moves.add_move(moving, from, to, captured, en_passant_capture, promoted_to);
	};
	//TODO generalize, obviously
	const bb white_pawns = p->pawns & p->white;
	const bb white_knights = p->knights & p->white;
	const bb white_bishops = p->bishops & p->white;
	const bb white_rooks = p->rooks & p->white;
	const bb white_queens = p->queens & p->white;
	const bb white_kings = p->kings & p->white;
	const bb black_pawns = p->pawns & p->black;
	const bb black_knights = p->knights & p->black;
	const bb black_bishops = p->bishops & p->black;
	const bb black_rooks = p->rooks & p->black;
	const bb black_queens = p->queens & p->black;
	const bb black_kings = p->kings & p->black;
	const bb occupied = p->white | p->black;

	if (p->white_to_move) {
		visit_capture_moves(white_knights, Bitboard::knight_moves, f, p->black, Piece::WHITE_KNIGHT);
		visit_non_capture_moves(white_knights, Bitboard::knight_moves, f, occupied, Piece::WHITE_KNIGHT);
		visit_capture_moves(white_kings, Bitboard::king_moves, f, p->black, Piece::WHITE_KING);
		visit_non_capture_moves(white_kings, Bitboard::king_moves, f, occupied, Piece::WHITE_KING);
		add_non_capture_ray_moves(moves, Piece::WHITE_QUEEN, white_queens, Bitboard::rook_moves, occupied);
		add_non_capture_ray_moves(moves, Piece::WHITE_ROOK, white_rooks, Bitboard::rook_moves, occupied);

		visit_capture_ray_moves(white_queens, Bitboard::rook_moves, f, occupied, p->black, Piece::WHITE_QUEEN);
		visit_capture_ray_moves(white_rooks, Bitboard::rook_moves, f, occupied, p->black, Piece::WHITE_ROOK);
		visit_capture_ray_moves(white_bishops, Bitboard::bishop_moves, f, occupied, p->black, Piece::WHITE_BISHOP);
		visit_capture_ray_moves(white_queens, Bitboard::bishop_moves, f, occupied, p->black, Piece::WHITE_QUEEN);
		add_non_capture_ray_moves(moves, Piece::Piece::WHITE_BISHOP, white_bishops, Bitboard::bishop_moves, occupied);
		add_non_capture_ray_moves(moves, Piece::WHITE_QUEEN, white_queens, Bitboard::bishop_moves, occupied);
		generate_castling(f, true);
		visit_pawn_caps(white_pawns, Bitboard::white_pawn_capture_moves, f, p->black, Piece::WHITE_PAWN);
		visit_pawn_nocaps(white_pawns, Bitboard::white_pawn_no_capture_moves, f, occupied, Piece::WHITE_PAWN, true);
	}
	else {
		visit_capture_moves(black_knights, Bitboard::knight_moves, f, p->white, Piece::BLACK_KNIGHT);
		visit_non_capture_moves(black_knights, Bitboard::knight_moves, f, occupied, Piece::BLACK_KNIGHT);
		visit_capture_moves(black_kings, Bitboard::king_moves, f, p->white, Piece::BLACK_KING);
		visit_non_capture_moves(black_kings, Bitboard::king_moves, f, occupied, Piece::BLACK_KING);
		add_non_capture_ray_moves(moves, Piece::BLACK_QUEEN, black_queens, Bitboard::rook_moves, occupied);
		add_non_capture_ray_moves(moves, Piece::BLACK_ROOK, black_rooks, Bitboard::rook_moves, occupied);

		visit_capture_ray_moves(black_queens, Bitboard::rook_moves, f, occupied, p->white, Piece::BLACK_QUEEN);
		visit_capture_ray_moves(black_rooks, Bitboard::rook_moves, f, occupied, p->white, Piece::BLACK_ROOK);
		visit_capture_ray_moves(black_bishops, Bitboard::bishop_moves, f, occupied, p->white, Piece::BLACK_BISHOP);
		visit_capture_ray_moves(black_queens, Bitboard::bishop_moves, f, occupied, p->white, Piece::BLACK_QUEEN);
		add_non_capture_ray_moves(moves, Piece::Piece::BLACK_BISHOP, black_bishops, Bitboard::bishop_moves, occupied);
		add_non_capture_ray_moves(moves, Piece::BLACK_QUEEN, black_queens, Bitboard::bishop_moves, occupied);

		generate_castling(f, false);
		visit_pawn_caps(black_pawns, Bitboard::black_pawn_capture_moves, f, p->white, Piece::BLACK_PAWN);
		visit_pawn_nocaps(black_pawns, Bitboard::black_pawn_no_capture_moves, f, occupied, Piece::BLACK_PAWN, false);

	}
	return moves;
}
Move_container Move_generator::generate_pseudolegal_captures(Position position, size_t depth) {
	p = &position;

	Move_container& moves = Move_container::get(depth);
	//moves.reserve(35);
	moves.reset();
	const move_visitor& f =
		[&moves, this](const int8_t& moving, const uint8_t& from, const uint8_t& to, const int8_t& captured, const int8_t& promoted_to) {

		bool en_passant_capture = will_be_en_passant(to, moving);
		moves.add_move(moving, from, to, captured, en_passant_capture, promoted_to);
	};
	//TODO generalize, obviously
	const bb white_pawns = p->pawns & p->white;
	const bb white_knights = p->knights & p->white;
	const bb white_bishops = p->bishops & p->white;
	const bb white_rooks = p->rooks & p->white;
	const bb white_queens = p->queens & p->white;
	const bb white_kings = p->kings & p->white;
	const bb black_pawns = p->pawns & p->black;
	const bb black_knights = p->knights & p->black;
	const bb black_bishops = p->bishops & p->black;
	const bb black_rooks = p->rooks & p->black;
	const bb black_queens = p->queens & p->black;
	const bb black_kings = p->kings & p->black;
	const bb occupied = p->white | p->black;

	if (p->white_to_move) {
		visit_capture_moves(white_knights, Bitboard::knight_moves, f, p->black, Piece::WHITE_KNIGHT);
		//visit_non_capture_moves(white_knights, Bitboard::knight_moves, f, occupied, Piece::WHITE_KNIGHT);
		visit_capture_moves(white_kings, Bitboard::king_moves, f, p->black, Piece::WHITE_KING);
		//visit_non_capture_moves(white_kings, Bitboard::king_moves, f, occupied, Piece::WHITE_KING);
		//add_non_capture_ray_moves(moves, Piece::WHITE_QUEEN, white_queens, Bitboard::rook_moves, occupied);
		//add_non_capture_ray_moves(moves, Piece::WHITE_ROOK, white_rooks, Bitboard::rook_moves, occupied);

		visit_capture_ray_moves(white_queens, Bitboard::rook_moves, f, occupied, p->black, Piece::WHITE_QUEEN);
		visit_capture_ray_moves(white_rooks, Bitboard::rook_moves, f, occupied, p->black, Piece::WHITE_ROOK);
		visit_capture_ray_moves(white_bishops, Bitboard::bishop_moves, f, occupied, p->black, Piece::WHITE_BISHOP);
		visit_capture_ray_moves(white_queens, Bitboard::bishop_moves, f, occupied, p->black, Piece::WHITE_QUEEN);
		//add_non_capture_ray_moves(moves, Piece::Piece::WHITE_BISHOP, white_bishops, Bitboard::bishop_moves, occupied);
		//add_non_capture_ray_moves(moves, Piece::WHITE_QUEEN, white_queens, Bitboard::bishop_moves, occupied);
		//generate_castling(f, true);
		visit_pawn_caps(white_pawns, Bitboard::white_pawn_capture_moves, f, p->black, Piece::WHITE_PAWN);
		//visit_pawn_nocaps(white_pawns, Bitboard::white_pawn_no_capture_moves, f, occupied, Piece::WHITE_PAWN, true);
	}
	else {
		visit_capture_moves(black_knights, Bitboard::knight_moves, f, p->white, Piece::BLACK_KNIGHT);
		//visit_non_capture_moves(black_knights, Bitboard::knight_moves, f, occupied, Piece::BLACK_KNIGHT);
		visit_capture_moves(black_kings, Bitboard::king_moves, f, p->white, Piece::BLACK_KING);
		//visit_non_capture_moves(black_kings, Bitboard::king_moves, f, occupied, Piece::BLACK_KING);
		//add_non_capture_ray_moves(moves, Piece::BLACK_QUEEN, black_queens, Bitboard::rook_moves, occupied);
		//add_non_capture_ray_moves(moves, Piece::BLACK_ROOK, black_rooks, Bitboard::rook_moves, occupied);

		visit_capture_ray_moves(black_queens, Bitboard::rook_moves, f, occupied, p->white, Piece::BLACK_QUEEN);
		visit_capture_ray_moves(black_rooks, Bitboard::rook_moves, f, occupied, p->white, Piece::BLACK_ROOK);
		visit_capture_ray_moves(black_bishops, Bitboard::bishop_moves, f, occupied, p->white, Piece::BLACK_BISHOP);
		visit_capture_ray_moves(black_queens, Bitboard::bishop_moves, f, occupied, p->white, Piece::BLACK_QUEEN);
		//add_non_capture_ray_moves(moves, Piece::Piece::BLACK_BISHOP, black_bishops, Bitboard::bishop_moves, occupied);
		//add_non_capture_ray_moves(moves, Piece::BLACK_QUEEN, black_queens, Bitboard::bishop_moves, occupied);

		//generate_castling(f, false);
		visit_pawn_caps(black_pawns, Bitboard::black_pawn_capture_moves, f, p->white, Piece::BLACK_PAWN);
		//visit_pawn_nocaps(black_pawns, Bitboard::black_pawn_no_capture_moves, f, occupied, Piece::BLACK_PAWN, false);

	}
	return moves;
}
bool Move_generator::is_attacked_by_slider(bb position,
	const bitboard_set& all_moves, const uint8_t& square,
	const bb& occupied) {
	while (position != 0) {
		const uint8_t& from = Bitboard::extract_and_remove_square(position);
		const bb& raw_moves = all_moves[from];
		bb kpsq = 0;
		Position::set_bit(kpsq, square);
		bb moves = raw_moves & kpsq;
		while (moves != 0x00) {
			const uint8_t& to = Bitboard::extract_and_remove_square(moves);
			const bool& b = Position::is_anything_between(from, to, occupied);
			if (!b) {
				return true;
			}
		}
	}
	return false;
}

bool Move_generator::is_attacked(const uint8_t& square) {

	const bb occupied = p->white | p->black;

	if (p->white_to_move) {
		const bb white_pawns = p->pawns & p->white;
		const bb white_knights = p->knights & p->white;
		const bb white_bishops = p->bishops & p->white;
		const bb white_rooks = p->rooks & p->white;
		const bb white_queens = p->queens & p->white;
		const bb white_kings = p->kings & p->white;

		if (is_attacked_by_pawn(white_pawns, Bitboard::black_pawn_capture_moves, square, true)) {
			return true;
		}
		if (Position::is_attacked_by_hopper(white_knights, Bitboard::knight_moves, square)) {
			return true;
		}
		if (Position::is_attacked_by_hopper(white_kings, Bitboard::king_moves, square)) {
			return true;
		}
		if (Position::is_attacked_by_slider(white_rooks | white_queens, Bitboard::rook_moves, square, occupied)) {
			return true;
		}
		if (Position::is_attacked_by_slider(white_bishops | white_queens, Bitboard::bishop_moves, square, occupied)) {
			return true;
		}
	}
	else {
		const bb black_pawns = p->pawns & p->black;
		const bb black_knights = p->knights & p->black;
		const bb black_bishops = p->bishops & p->black;
		const bb black_rooks = p->rooks & p->black;
		const bb black_queens = p->queens & p->black;
		const bb black_kings = p->kings & p->black;
		if (is_attacked_by_pawn(black_pawns, Bitboard::white_pawn_capture_moves, square, false)) {
			return true;
		}
		if (Position::is_attacked_by_hopper(black_knights, Bitboard::knight_moves, square)) {
			return true;
		}
		if (Position::is_attacked_by_hopper(black_kings, Bitboard::king_moves, square)) {
			return true;
		}
		if (is_attacked_by_slider(black_rooks | black_queens, Bitboard::rook_moves, square, occupied)) {
			return true;
		}
		if (is_attacked_by_slider(black_bishops | black_queens, Bitboard::bishop_moves, square, occupied)) {
			return true;
		}
	}
	return false;
}

bool Move_generator::is_attacked_by_pawn(const bb movers, const bitboard_set& all_moves, const uint8_t square, bool side_to_move) {
	bb raw_moves = all_moves[square];
	if ((side_to_move && square > 31) || (!side_to_move && square < 31)) { //TODO bit test rather than lt/gt
		raw_moves |= p->en_passant_square;
	}
	bb moves = raw_moves & movers;
	return moves != 0x00;
}
