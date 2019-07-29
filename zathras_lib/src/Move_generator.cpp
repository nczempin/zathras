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


namespace Moves {
	Move_generator::Move_generator() {
	}

	Move_generator::~Move_generator() {
		// TODO Auto-generated destructor stub
	}
	bool Move_generator::init_between() {
		uint8_t x = 0;
		uint8_t y = 0;
		for (uint8_t i = 0; i < 64; ++i) {
			for (uint8_t j = 0; j < 64; ++j) {
				bb poss = possibly_between_pre(square_t(i), square_t(j)); // TODO: more cleanly handle the uint8_t vs. square_t (enum)
				y = i;
				x = j;
				size_t index = Position::calc_index(x, y);
				Position::between[index] = poss;
			}
		}

		return true; //TODO why do I need a return value at all?
	}
	bool Move_generator::between_initialized = init_between(); // TODO workaround for missing static initializer block

	int8_t Move_generator::find_captured_piece(square_t square, int8_t moving) {
		int8_t captured = 0;
		if (is_set_square(p->pawns, square)) {
			captured = Piece::PAWN;
		}
		else if (is_set_square(p->knights, square)) {
			captured = Piece::KNIGHT;
		}
		else if (is_set_square(p->bishops, square)) {
			captured = Piece::BISHOP;
		}
		else if (is_set_square(p->rooks, square)) {
			captured = Piece::ROOK;
		}
		else if (is_set_square(p->queens, square)) {
			captured = Piece::QUEEN;
		}
		else if (is_set_square(p->kings, square)) {
			captured = Piece::KING; // should this even happen? exception?
		}
		if (is_set_square(p->black, square)) { //black
			captured = -captured;
		}
		else {
			//if (!is_set_square(p->white, square)) {
			//	if (p->en_passant_square == 0) {
			//		p->debug_position();
			//		throw 68;
			//	}
			//}
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
		//if ((moving < 0 && captured < 0) || (moving > 0 && captured > 0)) {
		//	p->debug_position();
		//	throw 27;
		//}

		return captured;
	}
	bool Move_generator::has_captured_piece(square_t square, int8_t moving) {

		if (is_set_square(p->black | p->white, square)) { //black
			return true;
		}

		if (p->en_passant_square != 0) {

			//TODO
			//Bitboard::visit_bitboard(p->en_passant_square,
			//	[&square, &moving](square_t en_passant_capture_square) {
			//		if (square == en_passant_capture_square) {
			//			if (moving == Piece::BLACK_PAWN) {
			//				return true;
			//			}
			//			if (moving == Piece::WHITE_PAWN && en_passant_capture_square > 31) { // in black half, must be black pawn
			//				return true;
			//			}
			//		}
			//		return false;
			//	});
			//return false;
		}


		return false;
	}

	void Move_generator::visit_capture_moves(const bb& sub_position, const bitboard_set& all_moves, const move_visitor& f, const bb& other_colour, const int8_t& moving) {
		bb position = sub_position;
		while (position != 0) {
			const square_t from = square_t(Bitboard::extract_and_remove_square(position)); // TODO handle cast better
			if (from == 255) {
				break; //TODO
			}
			const bb raw_moves = all_moves[from];
			bb moves = raw_moves & other_colour;
			while (moves != 0x00) {
				square_t to = square_t(Bitboard::extract_and_remove_square(moves)); // TODO handle cast better
				f(from, to, NONE);
			}
		}
	}

	void Move_generator::visit_pawn_caps(const bb& sub_position, const bitboard_set& all_moves, const move_visitor& f, const bb& other_colour, const int8_t& moving) {
		bb position = sub_position;
		while (position != 0) {
			const square_t from = square_t(Bitboard::extract_and_remove_square(position)); // TODO handle cast better
			const bb& raw_moves = all_moves[from];

			if ((moving == Piece::BLACK_PAWN
				&& (p->en_passant_square & Bitboard::BB_RANK3))
				|| (moving == Piece::WHITE_PAWN
					&& (p->en_passant_square & Bitboard::BB_RANK6))) {
				bb moves = raw_moves & (p->en_passant_square);
				while (moves != 0x00) {
					square_t to = square_t(Bitboard::extract_and_remove_square(moves)); // TODO handle cast better
					f(from, to, Move_type::EN_PASSANT);
				}
			}
			bb moves = raw_moves & other_colour;
			while (moves != 0x00) {
				square_t to = square_t(Bitboard::extract_and_remove_square(moves)); // TODO handle cast better
				int8_t captured = has_captured_piece(to, moving);
				if (captured) {
					if (to >= 56) { // promoting white pawn
						f(from, to, NONE); //TODO find out how to use defaults on function objects
						//TODO switch subpromotions on/off here
	/*					f(moving, from, to, captured, Piece::WHITE_ROOK);
						f(moving, from, to, captured, Piece::WHITE_BISHOP);
						f(moving, from, to, captured, Piece::WHITE_KNIGHT);*/
						// end switch subpromotions on/off

					}
					else if (to <= 7) { // promoting black pawn
						f(from, to, NONE);
						//TODO switch subpromotions on/off here
						//f(moving, from, to, captured, Piece::BLACK_ROOK);
						//f(moving, from, to, captured, Piece::BLACK_BISHOP);
						//f(moving, from, to, captured, Piece::BLACK_KNIGHT);
						// end switch subpromotions on/off

					}
					else {
						f(from, to, NONE);
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
			const square_t& from = static_cast<square_t>(Bitboard::extract_and_remove_square(position));
			if (from == 255) { //TODO
				break;
			}
			const bb raw_moves = all_moves[from];
			bb moves = raw_moves & ~other_colour;
			//TODO move this to a separate visit_pawn_caps method
			while (moves != 0x00) {
				const square_t& to = static_cast<square_t>(Bitboard::extract_and_remove_square(moves));
				f(from, to, NONE); //TODO
			}
		}
	}


	void Move_generator::visit_non_capture_ray_moves(const bb& sub_position,
		const bitboard_set& all_moves, const move_visitor& f,
		const bb& occupied, const int8_t& moving) {
		bb position = sub_position;
		while (position != 0) {
			const square_t from = square_t(Bitboard::extract_and_remove_square(position)); // TODO proper cast
			const bb raw_moves = all_moves[from];
			bb moves = raw_moves & ~occupied;
			while (moves != 0x00) {
				square_t to = square_t(Bitboard::extract_and_remove_square(moves)); // TODO proper cast
				bool b = Position::is_anything_between(from, to, occupied);
				if (!b) {
					f(from, to, NONE);
				}
			}
		}
	}
	void Move_generator::visit_capture_ray_moves(const bb& sub_position, const bitboard_set& all_moves, const move_visitor& f, const bb& occupied, const bb& other_colour) {
		bb position = sub_position;
		while (position != 0) {
			const square_t from = square_t(Bitboard::extract_and_remove_square(position)); // TODO proper cast
			const bb raw_moves = all_moves[from];
			bb moves = raw_moves & other_colour;
			while (moves != 0x00) {
				square_t to = square_t(Bitboard::extract_and_remove_square(moves)); // TODO proper cast
				bool b = Position::is_anything_between(from, to, occupied);
				if (!b) {
					//int8_t captured = find_captured_piece(to, moving);
					f(from, to, NONE);
				}
			}
		}

	}
	void Move_generator::visit_moves_raw(const bb sub_position,
		const bitboard_set all_moves, move_visitor f, int8_t moving) {

		Bitboard::visit_bitboard(sub_position,
			[&all_moves, &f, &moving](square_t x) {
				Bitboard::visit_bitboard(all_moves[x], [&x, &f, &moving](square_t y) {
					int8_t captured = -98;
					f(x, y, NONE);
					});

			});
	}

	bb Move_generator::filter_occupied_squares_for_pawns(bool white_to_move, bb occupied, const bitboard_set& all_moves, int x) {
		//TODO simplify the distinction between white's move and black's move
		const bb jump_over = white_to_move ? Bitboard::BB_RANK3 : Bitboard::BB_RANK6;
		bb jump_over_occupied = jump_over & occupied;
		bb jump_over_occupants_shifted = white_to_move ? jump_over_occupied << 8 : jump_over_occupied >> 8;
		bb all_moves_from_here = all_moves[x];
		const bb jump_onto = white_to_move ? Bitboard::BB_RANK4 : Bitboard::BB_RANK5;
		bb all_moves_to_fifth = (x >= 48 && x < 56) ? all_moves_from_here & jump_onto : 0;
		bb all_moves_to_fourth = (x >= 8 && x < 16) ? all_moves_from_here & jump_onto : 0;
		bb all_moves_to_jump = white_to_move ? all_moves_to_fourth : all_moves_to_fifth;
		bb filter_out = all_moves_to_jump & jump_over_occupants_shifted;
		bb moves = (all_moves_from_here & ~occupied) & ~filter_out;
		return moves;
	}

	void Move_generator::visit_pawn_nocaps(const bb& sub_position,
		const bitboard_set& all_moves, const move_visitor& f,
		const bb& occupied, const int8_t& moving, const bool& white_to_move) {
		bb position = sub_position;
		while (position != 0) {
			const square_t from = square_t(Bitboard::extract_and_remove_square(position));
			bb moves = filter_occupied_squares_for_pawns(white_to_move, occupied, all_moves, from);
			while (moves != 0x00) {
				square_t to = square_t(Bitboard::extract_and_remove_square(moves));
				if (to >= 56) { // promoting white pawn
					f(from, to, NONE);
					//TODO switch subpromotions on/off here
					/*f(moving, from, to, 0, Piece::WHITE_ROOK);
					f(moving, from, to, 0, Piece::WHITE_BISHOP);
					f(moving, from, to, 0, Piece::WHITE_KNIGHT);*/
					// end switch subpromotions on/off

				}
				else if (to <= 7) { // promoting black pawn
					f(from, to, NONE);
					//TODO switch subpromotions on/off here
					//f(moving, from, to, 0, Piece::BLACK_ROOK);
					//f(moving, from, to, 0, Piece::BLACK_BISHOP);
					//f(moving, from, to, 0, Piece::BLACK_KNIGHT);
					// end switch subpromotions on/off

				}
				else {
					f(from, to, NONE);
				}
			}
		}
	}

	void Move_generator::add_pawn_nocaps2(Move_container& moves, const bb& sub_position, const bitboard_set& all_moves, const bb& occupied, const bool& white_to_move) {
		bb position = sub_position;

		//FIXME: this method is faulty, it lets the pawn jump over pieces on a two-step.
		while (position != 0) {
			const square_t from = square_t(Bitboard::extract_and_remove_square(position));
			bb moveses = filter_occupied_squares_for_pawns(white_to_move, occupied, all_moves, from); // TODO name :-P
			while (moveses != 0x00) {
				square_t to = square_t(Bitboard::extract_and_remove_square(moveses));
				if (to >= 56) { // promoting white pawn
					moves.add_move(from, to, NONE);
					//TODO switch subpromotions on/off here
					/*f(moving, from, to, 0, Piece::WHITE_ROOK);
					f(moving, from, to, 0, Piece::WHITE_BISHOP);
					f(moving, from, to, 0, Piece::WHITE_KNIGHT);*/
					// end switch subpromotions on/off

				}
				else if (to <= 7) { // promoting black pawn
					moves.add_move(from, to, NONE);
					//TODO switch subpromotions on/off here
					//f(moving, from, to, 0, Piece::BLACK_ROOK);
					//f(moving, from, to, 0, Piece::BLACK_BISHOP);
					//f(moving, from, to, 0, Piece::BLACK_KNIGHT);
					// end switch subpromotions on/off

				}
				else {
					moves.add_move(from, to, NONE);
				}
			}
		}
	}


	void Move_generator::add_pawn_nocaps(Move_container& moves, const bb& sub_position, const bitboard_set& all_moves, const bb& occupied, const bool& white_to_move) {
		//TODO subpromotions

		Position pos = *p;

		int8_t up = (white_to_move ? 8 : -8);

			const bb non_promoters = white_to_move ?
			(p->pawns & p->white) & ~Bitboard::BB_RANK7 :
			(p->pawns & p->black) & ~Bitboard::BB_RANK2;

		bb enemies = white_to_move ? p->black : p->white;
		const bb unoccupied = ~occupied;

		bb single_step = white_to_move ? non_promoters << 8 & unoccupied : non_promoters >> 8 & unoccupied;
		bb double_step = white_to_move ?
			(single_step & Bitboard::BB_RANK3) << 8 & unoccupied :
			(single_step & Bitboard::BB_RANK6) >> 8 & unoccupied;


		while (single_step)
		{
			square_t to = Bitboard::extract_and_remove_square(single_step);
			moves.add_move(static_cast<square_t>(to - up), to, NONE);
		}

		while (double_step)
		{
			square_t to = Bitboard::extract_and_remove_square(double_step);
			moves.add_move(static_cast<square_t>(to - 2 * up), to, NONE);
		}


		const bb potential_promoters = white_to_move ?
			(p->pawns & p->white) & Bitboard::BB_RANK7 :
			(p->pawns & p->black) & Bitboard::BB_RANK2;
		if (potential_promoters)
		{



			bb non_capture_promotions = white_to_move ?
				potential_promoters << 8 & unoccupied :
				potential_promoters >> 8 & unoccupied;



			while (non_capture_promotions) {
				square_t to = Bitboard::extract_and_remove_square(non_capture_promotions);
				moves.add_move(static_cast<square_t>(to - up), to, NONE); //TODO sub-promotions

			}

		}



		}


		void Move_generator::attempt_castle(const move_visitor f, const int8_t piece,
			const square_t king_square, const int8_t direction) {
			//1. check if squares between king and rook are free.
			square_t next_square = square_t(uint8_t(king_square) + direction); //TODO proper cast
			square_t target_square = square_t(uint8_t(king_square) + direction * 2);//TODO proper cast

			if (is_set_square(p->white | p->black, next_square)) {
				return;
			}
			if (is_set_square(p->white | p->black, target_square)) {
				return;
			}
			if (direction == -1) { // queen side
				if (is_set_square(p->white | p->black, square_t(target_square - 1))) { // single_step/b8
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

			f(king_square, square_t(uint8_t(king_square) + direction * 2), NONE);

		}

		void Move_generator::generate_castling(const move_visitor & f, bool white_to_move) {
			int8_t piece = Piece::WHITE_KING;
			square_t king_square = Squares::E1;
			if (!white_to_move) {
				piece = Piece::BLACK_KING;
				king_square = Squares::E8;
			}
			static int8_t king_jump_direction[] = { 1, -1, 1, -1 };
			static bool colour[] = { true, true, false, false };
			for (size_t i = 0; i < 4; ++i) {
				if (white_to_move == colour[i] && p->castling[i]) {
					attempt_castle(f, piece, king_square, king_jump_direction[i]);
				}
			}
		}
		void Move_generator::f(Move_container & moves, const int8_t moving,
			const square_t from, const square_t to, const int8_t captured,
			const int8_t promoted_to) {
			moves.add_move(from, to, NONE);
		}

		bool Move_generator::will_be_en_passant(square_t to, int8_t moving) {
			bool en_passant_capture = false;
			if (is_set_square(p->en_passant_square, to)) {
				if ((moving == Piece::WHITE_PAWN && to > 31)
					|| (moving == Piece::BLACK_PAWN && to < 31)) {
					en_passant_capture = true;
				}
			}
			return en_passant_capture;
		}

		void Move_generator::add_non_capture_ray_moves(Move_container & moves, bb position, const bitboard_set & pieceMoves, const bb & occupied) {
			while (position != 0) {
				const square_t& from = square_t(Bitboard::extract_and_remove_square(position));  //TODO cast
				const bb& raw_moves = pieceMoves[from]; //TODO potential bug flagged here: "C28020: The expression '0<=_Param_(1)&&_Param_(1)<=64-1' is not true at this call."
				bb moves_bb = raw_moves & ~occupied;
				while (moves_bb != 0x00) {
					const square_t& to = square_t(Bitboard::extract_and_remove_square(moves_bb));  //TODO cast
					const bool& b = Position::is_anything_between(from, to, occupied);
					if (!b) {
						moves.add_move(from, to, NONE);
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
				Move_state ms;
				position.make_move(move, ms);
				//TODO++Interface::Info::nodes;
				if (!position.is_in_check(!position.is_white_to_move())) {
					legal_moves.add_move(move);
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
			for (int i = 0; i < pseudolegal_moves.size(); ++i) {
				Move move = moves[i];
				Move_state ms;
				position.make_move(move, ms);
				//TODO++Interface::Info::nodes;

				if (!position.is_in_check(!position.is_white_to_move())) {
					legal_moves.add_move(move);
				}
				position.unmake_move(move, ms);
			}
			return legal_moves;
		}

		Move_container Move_generator::generate_pseudolegal_moves(Position position, size_t depth) {
			p = &position;

			Move_container moves = Move_container::get(depth);
			//moves.reserve(35);
			moves.reset();
			const move_visitor& f =
				[&moves](const square_t& from, const square_t& to, const Move_type& move_type) {

				moves.add_move(from, to, move_type);
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
				//visit_pawn_nocaps(white_pawns, Bitboard::white_pawn_no_capture_moves, f, occupied, Piece::WHITE_PAWN, true);
				add_pawn_nocaps(moves, white_pawns, Bitboard::white_pawn_no_capture_moves, occupied, true);
				visit_capture_moves(white_knights, Bitboard::knight_moves, f, p->black, Piece::WHITE_KNIGHT);
				visit_non_capture_moves(white_knights, Bitboard::knight_moves, f, occupied, Piece::WHITE_KNIGHT);
				visit_capture_moves(white_kings, Bitboard::king_moves, f, p->black, Piece::WHITE_KING);
				add_non_capture_ray_moves(moves, white_queens | white_rooks, Bitboard::rook_moves, occupied);

				visit_capture_ray_moves(white_queens | white_rooks, Bitboard::rook_moves, f, occupied, p->black);
				visit_capture_ray_moves(white_bishops | white_queens, Bitboard::bishop_moves, f, occupied, p->black);
				add_non_capture_ray_moves(moves, white_bishops | white_queens, Bitboard::bishop_moves, occupied);
				generate_castling(f, true);
				visit_pawn_caps(white_pawns, Bitboard::white_pawn_capture_moves, f, p->black, Piece::WHITE_PAWN);
				visit_non_capture_moves(white_kings, Bitboard::king_moves, f, occupied, Piece::WHITE_KING);
			}
			else {
				visit_pawn_caps(black_pawns, Bitboard::black_pawn_capture_moves, f, p->white, Piece::BLACK_PAWN);
				//			visit_pawn_nocaps(black_pawns, Bitboard::black_pawn_no_capture_moves, f, occupied, Piece::BLACK_PAWN, false);
				add_pawn_nocaps(moves, black_pawns, Bitboard::black_pawn_no_capture_moves, occupied, false);
				visit_capture_moves(black_knights, Bitboard::knight_moves, f, p->white, Piece::BLACK_KNIGHT);
				visit_non_capture_moves(black_knights, Bitboard::knight_moves, f, occupied, Piece::BLACK_KNIGHT);
				visit_capture_moves(black_kings, Bitboard::king_moves, f, p->white, Piece::BLACK_KING);
				visit_non_capture_moves(black_kings, Bitboard::king_moves, f, occupied, Piece::BLACK_KING);
				add_non_capture_ray_moves(moves, black_queens | black_rooks, Bitboard::rook_moves, occupied);

				visit_capture_ray_moves(black_queens | black_rooks, Bitboard::rook_moves, f, occupied, p->white);
				visit_capture_ray_moves(black_bishops | black_queens, Bitboard::bishop_moves, f, occupied, p->white);
				add_non_capture_ray_moves(moves, black_bishops | black_queens, Bitboard::bishop_moves, occupied);

				generate_castling(f, false);

			}
			return moves;
		}
		Move_container Move_generator::generate_pseudolegal_captures(Position position, size_t depth) {
			p = &position;

			Move_container& moves = Move_container::get(depth);
			moves.reset();
			const move_visitor& f =
				[&moves](const square_t& from, const square_t& to, const move_type_t& move_type) {
				moves.add_move(from, to, move_type);
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
				visit_capture_moves(white_kings, Bitboard::king_moves, f, p->black, Piece::WHITE_KING);

				visit_capture_ray_moves(white_queens, Bitboard::rook_moves, f, occupied, p->black);
				visit_capture_ray_moves(white_rooks, Bitboard::rook_moves, f, occupied, p->black);
				visit_capture_ray_moves(white_bishops, Bitboard::bishop_moves, f, occupied, p->black);
				visit_capture_ray_moves(white_queens, Bitboard::bishop_moves, f, occupied, p->black);
				visit_pawn_caps(white_pawns, Bitboard::white_pawn_capture_moves, f, p->black, Piece::WHITE_PAWN);
			}
			else {
				visit_capture_moves(black_knights, Bitboard::knight_moves, f, p->white, Piece::BLACK_KNIGHT);
				visit_capture_moves(black_kings, Bitboard::king_moves, f, p->white, Piece::BLACK_KING);

				visit_capture_ray_moves(black_queens, Bitboard::rook_moves, f, occupied, p->white);
				visit_capture_ray_moves(black_rooks, Bitboard::rook_moves, f, occupied, p->white);
				visit_capture_ray_moves(black_bishops, Bitboard::bishop_moves, f, occupied, p->white);
				visit_capture_ray_moves(black_queens, Bitboard::bishop_moves, f, occupied, p->white);

				visit_pawn_caps(black_pawns, Bitboard::black_pawn_capture_moves, f, p->white, Piece::BLACK_PAWN);

			}
			return moves;
		}
		bool Move_generator::is_attacked_by_slider(bb position,
			const bitboard_set & all_moves, const square_t & square,
			const bb & occupied) {
			//TODO, BIG TODO: all this stuff is incredibly repetitive and should really be done with templates.
			while (position != 0) {
				const square_t& from = square_t(Bitboard::extract_and_remove_square(position));
				const bb& raw_moves = all_moves[from];
				bb kpsq = 0;
				set_bit(kpsq, square);
				bb moves = raw_moves & kpsq;
				while (moves != 0x00) {
					const square_t& to = square_t(Bitboard::extract_and_remove_square(moves));
					const bool& b = Position::is_anything_between(from, to, occupied);
					if (!b) {
						return true;
					}
				}
			}
			return false;
		}

		bool Move_generator::is_attacked(const square_t & square) {
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

		bool Move_generator::is_attacked_by_pawn(const bb movers, const bitboard_set & all_moves, const square_t square, bool side_to_move) {
			bb raw_moves = all_moves[square];
			if ((side_to_move && square > 31) || (!side_to_move && square < 31)) { //TODO bit test rather than lt/gt
				raw_moves |= p->en_passant_square;
			}
			bb moves = raw_moves & movers;
			return moves != 0x00;
		}
	}