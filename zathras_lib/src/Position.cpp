/*
 * Position.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#include <sstream>
#include <vector>
#include <iomanip>
#include <array>
#include <cassert>

#include "Position.h"
#include "Square.h"
#include "Piece.h"
#include "typedefs.h"
#include "Move_generator.h"

#include "Bitboard.h"
namespace Positions {
	Position::Position() {
		for (int i = 0; i < 6; ++i) {
			piece_bb[i] = 0;
		}
		//TODO clear piece bitboards?

		//TODO statically initialize


	}

	//
	Position::~Position() {
		// TODO Auto-generated destructor stub
	}
	


	//array<bb, 9> Position::getPieceBitboards() const {
	//	array<bb, 9> retval;
	//	//TODO figure out what to do with [0]
	//	retval[0] = 0x00;
	//	retval[1] = pawns;
	//	retval[2] = knights;
	//	retval[3] = bishops;
	//	retval[4] = rooks;
	//	retval[5] = queens;
	//	retval[6] = kings;
	//	retval[7] = white;
	//	retval[8] = black;

	//	return retval;
	//}

	static constexpr bool determine_colour(int piece) {
		return piece > 0; // makes == 0 black, deal with it
	}

	static inline constexpr piece_t determine_piece(piece_t piece) {
		return piece > 0 ? piece : -piece;
	}



	inline void Position::save_en_passant_square(Move_state& move_state) {
		if (en_passant_square != 0x00) {
			uint8_t eps = Bitboard::extract_square(en_passant_square);
			move_state.set_en_passant_square(eps);
			en_passant_square = 0x00;
		}
	}
	void Position::restore_en_passant_square(const Move_state& move_state) {
		en_passant_square = move_state.get_en_passant_square();
	}

	void Position::promote(int8_t promoted_to, square_t to) {
		//TODO no idea whether this is faster or simply using absolute value is
		switch (promoted_to) {
		case Piece::WHITE_QUEEN:
		case Piece::BLACK_QUEEN:
			Square::set_bit(queens, to);
			break;
		case Piece::WHITE_ROOK:
		case Piece::BLACK_ROOK:
			Square::set_bit(rooks, to);
			break;
		case Piece::WHITE_BISHOP:
		case Piece::BLACK_BISHOP:
			Square::set_bit(bishops, to);
			break;
		case Piece::WHITE_KNIGHT:
		case Piece::BLACK_KNIGHT:
			Square::set_bit(knights, to);
			break;
		}
	}
	void Position::un_promote(int8_t promoted_to, square_t to) {
		switch (promoted_to) {
		case Piece::WHITE_QUEEN:
		case Piece::BLACK_QUEEN:
			Square::clear_bit(queens, to);
			break;
		case Piece::WHITE_ROOK:
		case Piece::BLACK_ROOK:
			Square::clear_bit(rooks, to);
			break;
		case Piece::WHITE_BISHOP:
		case Piece::BLACK_BISHOP:
			Square::clear_bit(bishops, to);
			break;
		case Piece::WHITE_KNIGHT:
		case Piece::BLACK_KNIGHT:
			Square::clear_bit(knights, to);
			break;
		}

	}

	void Position::handle_capture(const square_t& to, const int8_t& taken,
		Move_state& move_state) {
		move_state.captured = taken;
		switch (taken) {
		case Piece::WHITE_PAWN:
			Square::clear_bit(white, to);
			Square::clear_bit(pawns, to);
			break;
		case Piece::WHITE_KNIGHT:
			Square::clear_bit(white, to);
			Square::clear_bit(knights, to);
			break;
		case Piece::WHITE_BISHOP:
			Square::clear_bit(white, to);
			Square::clear_bit(bishops, to);
			break;
		case Piece::WHITE_ROOK:
			Square::clear_bit(white, to);
			//TODO use constants, not magics
			if (castling[0] && to == 7) {
				castling[0] = false;
				move_state.set_cleared_kingside_castling(true);
			}
			else if (castling[1] && to == 0) {
				castling[1] = false;
				move_state.set_cleared_queenside_castling(true);
			}

			Square::clear_bit(rooks, to);
			break;
		case Piece::WHITE_QUEEN:
			Square::clear_bit(white, to);
			Square::clear_bit(queens, to);
			break;
		case Piece::WHITE_KING:
			Square::clear_bit(white, to);
			Square::clear_bit(kings, to);
			break;
		case Piece::BLACK_PAWN:
			Square::clear_bit(black, to);
			Square::clear_bit(pawns, to);
			break;
		case Piece::BLACK_KNIGHT:
			Square::clear_bit(black, to);
			Square::clear_bit(knights, to);
			break;
		case Piece::BLACK_BISHOP:
			Square::clear_bit(black, to);
			Square::clear_bit(bishops, to);
			break;
		case Piece::BLACK_ROOK:
			Square::clear_bit(black, to);
			//TODO use constants, not magics
			if (castling[2] && to == 63) {
				castling[2] = false;
				move_state.set_cleared_kingside_castling(true);
			}
			else if (castling[3] && to == 56) {
				castling[3] = false;
				move_state.set_cleared_queenside_castling(true);
			}
			Square::clear_bit(rooks, to);
			break;
		case Piece::BLACK_QUEEN:
			Square::clear_bit(black, to);
			Square::clear_bit(queens, to);
			break;
		case Piece::BLACK_KING:
			Square::clear_bit(black, to);
			Square::clear_bit(kings, to);
			break;
		default:
			cerr << "mmc??" << to_string(taken) << "\n";
			throw taken;
		}
	}

	void Position::make_move(const Move& move, Move_state& move_state) { //TODO move state as return value?
		bool set_en_passant = false;
		const square_t& from = move.get_from();
		const square_t& to = move.get_to();
		int8_t moving = get_piece_on(from);
		//if (moving == 0) {
		//	debug_position();
		//}
		assert(moving != 0);

		const int8_t& taken = get_piece_on(to);
		if (taken != 0) {
			//cout << "capturing: " << taken << endl;
			handle_capture(to, taken, move_state);
		}
		if (white_to_move) {
			make_move_for_colour<true>(from, to, moving, move, move_state, set_en_passant);
		}
		else {
			make_move_for_colour<false>(from, to, moving, move, move_state, set_en_passant);

		}
		if (!set_en_passant) {
			en_passant_square = 0x00;
		}
		// TODO update 3 repetitions
		// TODO update 50 moves
		white_to_move = !white_to_move;
	}


	void Position::unmake_move(const Move& move, const Move_state& move_state) {
		//TODO such a mess
		square_t from = move.get_from();
		square_t to = move.get_to();
		piece_t moving = get_piece_on(to);
		assert(moving != 0);

		board[from] = moving;
		restore_en_passant_square(move_state);
		white_to_move = !white_to_move;

		if (white_to_move) {
			Square::clear_bit(white, to);
			Square::set_bit(white, from);
			bb& pbb = piece_bb[moving - 1];
			Square::clear_bit(pbb, to);
			Square::set_bit(pbb, from);
			switch (moving) {
			case Piece::WHITE_PAWN: {
				// move pawn back
				//TODO clearing can be saved when move was a capture. find out which is faster
				if (is_in_back_rank_black(to)) {
					//TODO move_type = promotion
					//piece_t promoted_to = Piece::WHITE_QUEEN; //TODO allow underpromotion
					//un_promote(promoted_to, to);
					Square::clear_bit(queens, to);
				}
				else {
					// handle capturing by e. p.
					if (move.get_move_type() == EN_PASSANT) {
						square_t target = square_t(to - 8); //TODO
						en_passant_square = 0;
						Square::set_bit(en_passant_square, to);
						Square::set_bit(pawns, target);
						Square::set_bit(black, target);
						board[target] = Piece::BLACK_PAWN;
						board[to] = 0;
					}

				}
				break;
			}
			case Piece::WHITE_KNIGHT:
			case Piece::WHITE_BISHOP:
			case Piece::WHITE_QUEEN:
				break;
			case Piece::WHITE_ROOK:
				if (move_state.is_cleared_kingside_castling()) {
					castling[0] = true;
				}
				if (move_state.is_cleared_queenside_castling()) {
					castling[1] = true;
				}
				break;
			case Piece::WHITE_KING:
				if (from == E1 && to == C1) { //queenside castle
					Square::update_bits(white, rooks, D1, A1);
					board[D1] = 0;
					board[A1] = Piece::WHITE_ROOK;
				}
				else if (from == E1 && to == G1) { // kingside castle
					Square::update_bits(white, rooks, F1, H1);
					board[F1] = 0;
					board[H1] = Piece::WHITE_ROOK;
				}
				if (move_state.is_cleared_kingside_castling()) {
					castling[0] = true;
				}
				if (move_state.is_cleared_queenside_castling()) {
					castling[1] = true;
				}
				break;
			default:
				debug_position();
				cerr << "unexpected white piece: " << ((int)moving) << endl;
				throw moving;
				break;
			}
		}
		else {
			Square::clear_bit(black, to); //TODO unnecessary when capture
			Square::set_bit(black, from);
			bb& pbb = piece_bb[-moving - 1];
			Square::clear_bit(pbb, to); //TODO clear and set in one op when capture?
			Square::set_bit(pbb, from);
			switch (moving) {
			case Piece::BLACK_PAWN: {

				if (is_in_back_rank_white(to)) {
					//un_promote(Piece::BLACK_QUEEN, to);
					Square::clear_bit(queens, to);
				}
				else {
					// handle capturing by e. p.
					if (move.get_move_type() == EN_PASSANT) {
						square_t target = square_t(to + 8); //TODO
						en_passant_square = 0;
						Square::set_bit(en_passant_square, to);
						Square::set_bit(pawns, target);
						Square::set_bit(white, target);
						board[target] = Piece::WHITE_PAWN;
						board[to] = 0;
					}
				}
			}

									break;
			case Piece::BLACK_KNIGHT:
			case Piece::BLACK_BISHOP:
			case Piece::BLACK_QUEEN:
				break;
			case Piece::BLACK_ROOK:
				if (move_state.is_cleared_kingside_castling()) {
					castling[2] = true;
				}
				if (move_state.is_cleared_queenside_castling()) {
					castling[3] = true;
				}
				break;
			case Piece::BLACK_KING:
				if (from == E8 && to == C8) { //queenside castle
					Square::update_bits(black, rooks, D8, A8);//TODO constants, not magics
					board[D8] = 0;
					board[A8] = Piece::BLACK_ROOK;
				}
				else if (from == E8 && to == G8) { // kingside castle
					Square::update_bits(black, rooks, F8, H8);//TODO constants, not magics
					board[F8] = 0;
					board[H8] = Piece::BLACK_ROOK;
				}
				if (move_state.is_cleared_kingside_castling()) {
					castling[2] = true;
				}
				if (move_state.is_cleared_queenside_castling()) {
					castling[3] = true;
				}
				break;
			default:
				debug_position();
				double error = ((double)moving);
				cerr << "umm: unexpected black piece: " << error << endl;
				throw - moving;
				break;
			}
		}
		assert(get_piece_on(move.get_from()) != 0);
		if (move.get_move_type() != EN_PASSANT) {

			int8_t captured = move_state.captured;// get_captured();
			//board[from] = moving; //TODO encapsulate
			board[to] = captured; //TODO encapsulate

			if (captured != 0) {
				//cout << "untaken: " << (int)captured << endl;
				bool colour = determine_colour(captured);
				if (colour) {
					Square::set_bit(white, to);
				}
				else {
					Square::set_bit(black, to);
				}
				int8_t p = determine_piece(captured);
				switch (p) {
				case 1:
					Square::set_bit(pawns, to);
					break;
				case 2:
					Square::set_bit(knights, to);
					break;
				case 3:
					Square::set_bit(bishops, to);
					break;
				case Piece::ROOK:
					//TODO use constants, not magics
					if (move_state.is_cleared_kingside_castling() && to == 63) {
						castling[2] = true;
					}
					else if (move_state.is_cleared_queenside_castling()) {
						castling[3] = true;
					}
					else if (move_state.is_cleared_kingside_castling() && to == 7) {
						castling[0] = true;
					}
					else if (move_state.is_cleared_queenside_castling() && to == 0) {
						castling[1] = true;
					}
					Square::set_bit(rooks, to);
					break;
				case 5:
					Square::set_bit(queens, to);
					break;
				case 6:
					Square::set_bit(kings, to);
					break;
				default:
					cerr << "un??" << p << endl;
					throw p;
				}
				assert(get_piece_on(move.get_from()) != 0);
				assert(get_piece_on(move.get_to()) != 0);
			}
		}


		assert(get_piece_on(move.get_from()) != 0);
		// TODO update 3 repetitions
		// TODO update 50 moves
	}



	bool Position::is_in_check(const bool side) {
		//TODO this is a somewhat naive way of doing this, it needs to be much more efficient
		const bb colour = side ? white : black;
		const bb kpbb = kings & colour;
		//if (kpbb == 0) { //TODO debug flag
		//	cout << print_bitboard(kings);
		//	cout << print_bitboard(colour);
		//	cout << print_bitboard(white);
		//	cout << print_bitboard(black);
		//}
		assert(kpbb != 0);
		const square_t& king_pos = square_t(Bitboard::extract_square(kpbb));
		assert(king_pos < 64);
		if (king_pos >= 64) {
			debug_position();
			throw king_pos;
		}
		if (white_to_move) {
			const bb white_knights = knights & white;
			if (is_attacked_by_hopper(white_knights, Bitboard::knight_moves, king_pos)) {
				return true;
			}
			const bb white_pawns = pawns & white;
			if (is_attacked_by_hopper(white_pawns, Bitboard::black_pawn_capture_moves,
				king_pos)) {
				return true;
			}
			const bb occupied = black | white;
			if (is_check_from_slider(Bitboard::bishop_moves, king_pos, white & (bishops | queens),
				occupied)) {
				return true;
			}
			if (is_check_from_slider(Bitboard::rook_moves, king_pos, white & (rooks | queens), occupied)) {
				return true;
			}
			if (is_attacked_by_hopper(kings & white, Bitboard::king_moves, king_pos)) {
				return true;
			}
		}
		else {
			const bb black_knights = knights & black;
			if (is_attacked_by_hopper(black_knights, Bitboard::knight_moves, king_pos)) {
				return true;
			}
			const bb black_pawns = pawns & black;
			if (is_attacked_by_hopper(black_pawns, Bitboard::white_pawn_capture_moves,
				king_pos)) {
				return true;
			}
			const bb occupied = black | white;
			if (is_check_from_slider(Bitboard::bishop_moves, king_pos, black & (bishops | queens),
				occupied)) {
				return true;
			}
			if (is_check_from_slider(Bitboard::rook_moves, king_pos, black & (rooks | queens), occupied)) {
				return true;
			}
			if (is_attacked_by_hopper(kings & black, Bitboard::king_moves, king_pos)) {
				return true;
			}
		}
		return false;
	}

	bool Position::is_attacked_by_slider(bb position,
		const bitboard_set& all_moves, const square_t& square,
		const bb& occupied) {
		while (position != 0) {
			const square_t& from = square_t(Bitboard::extract_and_remove_square(position));
			const bb& raw_moves = all_moves[from];
			bb kpsq = 0;
			Square::set_bit(kpsq, square);
			bb moves = raw_moves & kpsq;
			while (moves != 0x00) {
				const square_t& to = square_t(Bitboard::extract_and_remove_square(moves));
				const bool& b = is_anything_between(from, to, occupied);
				if (!b) {
					return true;
				}
			}
		}
		return false;
	}


	bool Position::is_check_from_slider(const bitboard_set& sliding_moves,
		const square_t& king_pos, const bb& slider, const bb& occupied) {
		bool retval = false;
		const bb& raw_moves = sliding_moves[king_pos];
		const bb& moves = raw_moves & slider;
		if (moves != 0) {
			bb tmp = moves;
			uint8_t attacker = 0;
			uint8_t l = 0;
			while (tmp) {
				l = Bitboard::ffs(tmp);
				attacker = l;
				if (!is_anything_between(king_pos, square_t(attacker), occupied)) {
					return true;
				}
				tmp &= tmp - 1ULL; //clear LS1B
			}
		}
		return retval;
	}
	bool Position::is_attacked_by_hopper(const bb& movers,
		const bitboard_set& all_moves, const square_t& square) {
		const bb& raw_moves = all_moves[square];
		const bb moves = raw_moves & movers;
		return moves != 0x00;
	}

	bool Position::is_anything_between(const square_t from, const square_t to, const bb& occupied) {
		uint16_t index = calc_index(from, to);
		return between[index] & occupied;
	}

	bb Position::between[BETWEEN_ARRAY_SIZE];


	template<bool white_or_not>
	inline void Position::make_move_for_colour(const square_t& from, const square_t& to, const int8_t& moving, const Move& move, Move_state& move_state, bool& set_en_passant) {
		{
			board[from] = 0; //TODO encapsulate
			board[to] = moving; //TODO encapsulate
			assert(moving != 0);
			if (white_or_not) {
				Square::clear_bit(white, from);
				Square::set_bit(white, to);
				bb& pbb = piece_bb[moving - 1];
				Square::set_bit(pbb, to);
				Square::clear_bit(pbb, from);

				switch (moving) {
				case Piece::WHITE_PAWN: {
					//clear_bit(pawns, from);

					if (is_in_back_rank_black(to)) { // target is rank 8 -> promote
													 //promote(Piece::WHITE_QUEEN, to);
						Square::set_bit(queens, to);
						Square::clear_bit(pawns, to);
						board[to] = Piece::WHITE_QUEEN;
					}
					else {
						// handle capturing by e. p.

						//TODO: find out which method is faster
						//if (get_piece_on(to) == 0xff) { // en passant capture
						if (move.get_move_type() == EN_PASSANT) { // en passant capture
							square_t target = square_t(to - 8);
							Square::clear_bit(pawns, target);
							Square::clear_bit(black, target);
							board[target] = 0;
						}
						else {
							save_en_passant_square(move_state);
							// handle double step preparing the e. p.
							if (to - from == 16) {
								square_t target = square_t(to - 8);
								set_en_passant = true;
								Square::set_bit(en_passant_square, target);
							}
						}
					}
					break;
				}
				case Piece::WHITE_KNIGHT:
				case Piece::WHITE_BISHOP:
				case Piece::WHITE_QUEEN:
				{

					save_en_passant_square(move_state);
					break;
				}
				case Piece::WHITE_ROOK:
					if (from == 7 && castling[0]) { // H1 //TODO
						castling[0] = false;
						move_state.set_cleared_kingside_castling(true);
					}
					else if (from == 0 && castling[1]) { //A1 // TODO
						castling[1] = false;
						move_state.set_cleared_queenside_castling(true);
					}
					save_en_passant_square(move_state);
					break;
				case Piece::WHITE_KING:
					if (from == E1 && to == C1) { //queenside castle
						Square::update_bits(white, rooks, A1, D1);
						board[A1] = 0;
						board[D1] = Piece::WHITE_ROOK;
					}
					else if (from == E1 && to == G1) { // kingside castle
						Square::update_bits(white, rooks, H1, F1);
						board[H1] = 0;
						board[F1] = Piece::WHITE_ROOK;
					}
					if (castling[0]) {
						move_state.set_cleared_kingside_castling(true);
						castling[0] = false;
					}
					if (castling[1]) {
						move_state.set_cleared_queenside_castling(true);
						castling[1] = false;
					}
					save_en_passant_square(move_state);
					break;
				}
			}
			else {
				Square::clear_bit(black, from);
				Square::set_bit(black, to);
				bb& pbb = piece_bb[-moving - 1];
				Square::set_bit(pbb, to);
				Square::clear_bit(pbb, from);
				switch (moving) {
				case Piece::BLACK_PAWN: {
					//clear_bit(pawns, from);
					/*int8_t promoted_to = move.get_promoted_to();
					if (promoted_to != 0) {*/
					if (is_in_back_rank_white(to)) {
						Square::set_bit(queens, to);
						Square::clear_bit(pawns, to); // that was eagerly set previously
													  //							promote(Piece::BLACK_QUEEN, to); // TODO underpromote
					}
					else {

						// handle capturing by e. p.
						square_t target = square_t(to + 8);


						//TODO: find out which method is faster
						//if (get_piece_on(to) == 0xff) { // en passant capture
						if (move.get_move_type() == EN_PASSANT) { // en passant capture
							Square::clear_bit(pawns, target);
							Square::clear_bit(white, target);

							board[target] = 0;
						}
						else {
							// handle double step preparing the e. p.
							save_en_passant_square(move_state);
							if (to - from == -16) {
								Square::set_bit(en_passant_square, target);
								set_en_passant = true;
							}
						}
					}
					break;
				}
				case Piece::BLACK_KNIGHT:
				case Piece::BLACK_BISHOP:
				case Piece::BLACK_QUEEN:
				{

					save_en_passant_square(move_state);
					break;
				}
				case Piece::BLACK_ROOK:

					if (from == 63 && castling[2]) { // H8 //TODO
						move_state.set_cleared_kingside_castling(true);
						castling[2] = false;
					}
					else if (from == 56 && castling[3]) { //A8 // TODO
						move_state.set_cleared_queenside_castling(true);
						castling[3] = false;
					}
					save_en_passant_square(move_state);
					break;
				case Piece::BLACK_KING:


					if (from == E8 && to == C8) { //queenside castle
						Square::update_bits(black, rooks, A8, C8);
						board[A8] = 0;
						board[D8] = Piece::BLACK_ROOK;
					}
					else if (from == E8 && to == G8) { // kingside castle
						Square::update_bits(black, rooks, H8, F8);
						board[H8] = 0;
						board[F8] = Piece::BLACK_ROOK;
					}
					if (castling[2]) {
						move_state.set_cleared_kingside_castling(true);
						castling[2] = false;
					}
					if (castling[3]) {
						move_state.set_cleared_queenside_castling(true);
						castling[3] = false;
					}
					save_en_passant_square(move_state);
					break;
				}
			}
		}

	}
}