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
		static bool is_attacked_by_hopper(const bb& movers, const bitboard_set& all_moves, const square_t& square);
		static bool is_check_from_slider(const bitboard_set& sliding_moves, const square_t& king_pos, const bb& slider, const bb& occupied);

		static bool is_attacked_by_slider(bb position, const bitboard_set& all_moves, const square_t& square, const bb& occupied);
		static bool is_anything_between(square_t from, square_t to, const bb& occupied);

		//TODO template
		static bool is_in_back_rank_black(square_t square) {
			return Squares::A8 <= square && square <= Squares::H8;
		}
		static bool is_in_back_rank_white(square_t square) {
			return Squares::A1 <= square && square <= Squares::H1;

		}
		piece_t get_piece_on(square_t sq) {
			return board[sq];
		}
		friend ostream& operator<<(ostream& stream, const Position& position);
		void print(ostream& stream) const;
		string print_board() const;
		static Position create_start_position();

		static vector<string> split(const string& s, char delim);



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

		static string extract_row_string(uint_fast8_t row, string set);
		static void display_all_moves(const bitboard_set& moves);
		void save_en_passant_square(Move_state& move_state);
		void restore_en_passant_square(const Move_state& move_state);
		void promote(int8_t promoted_to, square_t to);
		void un_promote(int8_t promoted_to, square_t to);
		void handleCapture(const square_t& to, const int8_t& taken, Move_state& move_state);


	public:
		static bb between[BETWEEN_ARRAY_SIZE];
		inline static uint16_t calc_index(const uint8_t& x, const uint8_t& y) {
			return x + 64 * y;
		}

		template<bool white_or_not> void make_move_for_colour(const square_t& from, const square_t& to, const int8_t& moving, const Move& move, Move_state& move_state, bool& set_en_passant);


	};

}
#endif /* POSITION_H_ */
