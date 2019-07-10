/*
 * Position.h
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */
#pragma once
#ifndef POSITION_H_
#define POSITION_H_

#include <bitset>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>

#include "typedefs.h"
#include "Move.h"
#include "Move_state.h"
#include "Bitboard.h"
#include "Piece.h"

 //#include "Info.h"
namespace Positions {
#define pawns piece_bb[0]
#define knights piece_bb[1]
#define bishops piece_bb[2]
#define rooks piece_bb[3]
#define queens piece_bb[4]
#define kings piece_bb[5]
	namespace {
		const int BETWEEN_ARRAY_SIZE = 64 * 64;//2017;
	}

	using namespace Moves;

	class Position {
		static const bitboard_set rook_moves;

	public:
		Position();
		virtual ~Position();

		static vector<string>& split(const string& s, char delim, vector<string>& elems);

		void make_move(const Move& move, Move_state& move_state);

		void unmake_move(const Move& move, const Move_state& move_state);

		bool is_in_check(const bool side);
		static bool is_attacked_by_hopper(const bb& movers, const bitboard_set& all_moves, const uint8_t& square);
		static bool is_check_from_slider(const bitboard_set& sliding_moves, const uint8_t& king_pos, const bb& slider, const bb& occupied);

		static bool is_attacked_by_slider(bb position, const bitboard_set& all_moves, const uint8_t& square, const bb& occupied);
		static bool is_anything_between(uint8_t from, uint8_t to, const bb& occupied);

		//TODO template
		static bool is_in_back_rank_black(square_t square) {
			return Square::A8 <= square && square <= Square::H8;
		}
		static bool is_in_back_rank_white(square_t square) {
			return Square::A1 <= square && square <= Square::H1;

		}
		piece_t get_piece_on(square_t sq) {
			return board[sq];

			//TODO all the rest is obsolete
			piece_t retval = 0;

			if (is_set_square(white, sq)) {
				if (is_set_square(pawns, sq)) {
					retval = Piece::WHITE_PAWN;
				}
				else if (is_set_square(knights, sq)) {
					retval = Piece::WHITE_KNIGHT;
				}
				else if (is_set_square(bishops, sq)) {
					retval = Piece::WHITE_BISHOP;
				}
				else if (is_set_square(rooks, sq)) {
					retval = Piece::WHITE_ROOK;
				}
				else if (is_set_square(queens, sq)) {
					retval = Piece::WHITE_QUEEN;
				}
				else if (is_set_square(kings, sq)) {
					retval = Piece::WHITE_KING;
				}
			}
			else if (is_set_square(black, sq)) {
				if (is_set_square(pawns, sq)) {
					retval = Piece::BLACK_PAWN;
				}
				else if (is_set_square(knights, sq)) {
					retval = Piece::BLACK_KNIGHT;
				}
				else if (is_set_square(bishops, sq)) {
					retval = Piece::BLACK_BISHOP;
				}
				else if (is_set_square(rooks, sq)) {
					retval = Piece::BLACK_ROOK;
				}
				else if (is_set_square(queens, sq)) {
					retval = Piece::BLACK_QUEEN;
				}
				else if (is_set_square(kings, sq)) {
					retval = Piece::BLACK_KING;
				}
			}
			else {
				// empty, stays 0
			}

			return retval;
		}
		friend ostream& operator<<(ostream& stream, const Position& position);
		void print(ostream& stream) const;
		string print_board() const;
		static Position create_start_position();

		static vector<string> split(const string& s, char delim);

		//TODO move; probably to Square class
		static void set_square(bitset<64> & bs, uint8_t to);
		static void clear_square(bitset<64> & bs, uint8_t to);
		static void set_square(bb& bs, uint8_t to);
		static bool is_set_square(bb bs, uint8_t to);
		static void clear_square(bb& bs, uint8_t to);
		static void set_bit(bb& bs, const uint8_t& to);
		static void preset_bit(bb& bs, const uint8_t& to);
		static void clear_bit(bb& bs, const uint8_t& to);
		static void set_square(const uint8_t& file, const uint8_t& rank, bb& bbs);
		static void clear_square(const uint8_t& file, const uint8_t& rank, bb& bbs);
		static string print_mailbox_board(const piece_t board[64]);

		static void visualize_bitboard(bb my_bb, ostream& stream);
		static string print_bitboard(bb my_bb);
		static void visualize_mailbox_board(const piece_t board[64], ostream& stream);
		static void visit_mailbox_board(const int board[64], void (*visitor)(int)); // TODO convert to c++11

		static Position create_position(const string& fen);
		//TODO a separate Bitboard (helper) class is probably best
		array<bb, 9> getPieceBitboards() const;

		bool is_white_to_move() const {
			return white_to_move;
		}


		//TODO public for now
		bb piece_bb[6] = {};
		bb white = 0x00;
		bb black = 0x00;
		bb en_passant_square = 0x00;
		bitset<4> castling;
		bool white_to_move = true; //TODO public for now

		void mailbox_from_bitboard(piece_t board[64]) const;

		void debugPosition();
	private:
		piece_t board[64]; //TODO use std::array
		static bb squares[64];

		static string extract_row_string(uint_fast8_t row, string set);
		static void display_all_moves(const bitboard_set& moves);
		void update_bits(bb& colour, bb& piece, uint8_t clear, uint8_t set);
		void save_en_passant_square(Move_state& move_state);
		void restore_en_passant_square(const Move_state& move_state);
		void promote(int8_t promoted_to, uint8_t to);
		void un_promote(int8_t promoted_to, uint8_t to);
		void handleCapture(const uint8_t& to, const int8_t& taken, Move_state& move_state);


	public:
		static bb between[BETWEEN_ARRAY_SIZE];
		inline static uint16_t calc_index(const uint8_t& x, const uint8_t& y) {
			return x + 64 * y;
		}

		template<bool white_or_not> void make_move_for_colour(const uint8_t& from, const uint8_t& to, const int8_t& moving, const Move& move, Move_state& move_state, bool& set_en_passant) {
			{
				board[from] = 0; //TODO encapsulate
				board[to] = moving; //TODO encapsulate
				if (white_or_not) {
					clear_bit(white, from);
					set_bit(white, to);
					bb& pbb = piece_bb[moving - 1];
					set_bit(pbb, to);
					clear_bit(pbb, from);
					if (moving != Piece::WHITE_PAWN) {
						save_en_passant_square(move_state); //TODO don't do this twice
					}
					switch (moving) {
					case Piece::WHITE_PAWN: {
						clear_bit(pawns, from);

						if (is_in_back_rank_black(to)) { // target is rank 8 -> promote
							promote(Piece::WHITE_QUEEN, to);
						}
						else {
							set_bit(pawns, to);
							// handle capturing by e. p.
							int target = to - 8;
							if (get_piece_on(to) == 0) { // en passant capture
								clear_bit(pawns, target);
								clear_bit(black, target);
							}
							else {
								save_en_passant_square(move_state);
								// handle double step preparing the e. p.
								if (to - from == 16) {
									set_en_passant = true;
									set_bit(en_passant_square, target);
								}
							}
						}
						break;
					}
					case Piece::WHITE_KNIGHT:
					case Piece::WHITE_BISHOP:
					case Piece::WHITE_QUEEN:
					{
						bb& pbb = piece_bb[moving - 1];
						set_bit(pbb, to);
						clear_bit(pbb, from);
						save_en_passant_square(move_state);
						break;
					}case Piece::WHITE_ROOK:
						if (from == 7 && castling[0]) { // H1 //TODO
							castling[0] = false;
							move_state.set_cleared_kingside_castling(true);
						}
						else if (from == 0 && castling[1]) { //A1 // TODO
							castling[1] = false;
							move_state.set_cleared_queenside_castling(true);
						}
						break;
					case Piece::WHITE_KING:
						if (to == from - 2) { //queenside castle
							update_bits(white, rooks, 0, 3);//TODO constants, not magics

						}
						else if (from == to - 2) { // kingside castle
							update_bits(white, rooks, 7, 5);//TODO constants, not magics
						}
						if (castling[0]) {
							move_state.set_cleared_kingside_castling(true);
							castling[0] = false;
						}
						if (castling[1]) {
							move_state.set_cleared_queenside_castling(true);
							castling[1] = false;
						}
						break;
					}
				}
				else {
					clear_bit(black, from);
					set_bit(black, to);
					switch (moving) {
					case Piece::BLACK_PAWN: {
						clear_bit(pawns, from);
						/*int8_t promoted_to = move.get_promoted_to();
						if (promoted_to != 0) {*/
						if (is_in_back_rank_white(to)) {
							promote(Piece::BLACK_QUEEN, to); // TODO underpromote
						}
						else {
							set_bit(pawns, to);
							// handle capturing by e. p.
							int target = to + 8;

							
							if (get_piece_on(to) == 0) { // en passant capture
								clear_bit(pawns, target);
								clear_bit(white, target);
							}
							else {
								// handle double step preparing the e. p.
								save_en_passant_square(move_state);
								if (to - from == -16) {
									set_bit(en_passant_square, target);
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
					    bb& pbb = piece_bb[-moving - 1];
						set_bit(pbb, to);
						clear_bit(pbb, from);
						save_en_passant_square(move_state);
						break;
					}
					case Piece::BLACK_ROOK:
						set_bit(rooks, to);
						clear_bit(rooks, from);
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
						update_bits(black, kings, from, to);
						if (to == from - 2) { //queenside castle
							update_bits(black, rooks, 56, 59);//TODO constants, not magics
						}
						else if (from == to - 2) { // kingside castle
							update_bits(black, rooks, 63, 61);//TODO constants, not magics
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


	};
}
#endif /* POSITION_H_ */
