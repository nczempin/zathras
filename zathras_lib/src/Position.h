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
#include <stack>

#include "typedefs.h"
#include "Move.h"
#include "Move_state.h"
#include "Bitboard.h"
#include "Piece.h"


namespace positions {

	//TODO find better way to do this
#define pawns  piece_bb[0]
#define knights piece_bb[1]
#define bishops piece_bb[2]
#define rooks piece_bb[3]
#define queens piece_bb[4]
#define kings piece_bb[5]

	namespace {
		const int BETWEEN_ARRAY_SIZE = 64 * 64;
	}

	using namespace zathras_lib::moves;
	using Move = uint16_t;//TODO
	class Position {

	public:
		Position();
		virtual ~Position();


		void make_move(const Move& move, Move_state& move_state) {
			make_move(move, move_state, nullptr);
		}

		void make_move(const Move& move, Move_state& move_state, std::stack<Move>* move_stack);

		void unmake_move(const Move& move, const Move_state& move_state);

		//bool is_in_check(const bool side);
		piece_t get_piece_on(square_t sq) {
			return board[sq];
		}


		// TODO too lazy to unify this. it may be fine just to leave the member public; I don't foresee changing the representation any time soon
		bool is_white_to_move() const {
			return white_to_move;
		}
		bool white_to_move = true;


		//TODO public for now
		bb piece_bb[6] = {};
		bb white = 0x00;
		bb black = 0x00;
		bb en_passant_square = 0x00;
		bitset<4> castling;

		//TODO: move all the static stuff out of the class
		static bool is_attacked_by_hopper(const bb& movers, const bitboard_set& all_moves, const square_t& square);
		static bool is_check_from_slider(const bitboard_set& sliding_moves, const square_t& king_pos, const bb& slider, const bb& occupied);

		bool is_in_check(const bool side) {
			return is_in_check(side, nullptr);
		}

		bool is_in_check(const bool side, std::stack<Move>* move_stack);

		static bool is_attacked_by_slider(bb position, const bitboard_set& all_moves, const square_t& square, const bb& occupied);
		static bool is_anything_between(square_t from, square_t to, const bb& occupied);


	private:
		template<bool white_or_not> void make_move_for_colour(const square_t& from, const square_t& to, const int8_t& moving, const Move& move, Move_state& move_state, bool& set_en_passant);
		piece_t board[64]; //TODO use std::array

		void save_en_passant_square(Move_state& move_state);
		void restore_en_passant_square(const Move_state& move_state);
		void promote(int8_t promoted_to, square_t to);
		void un_promote(int8_t promoted_to, square_t to);
		void handle_capture(const square_t& to, const int8_t& taken, Move_state& move_state);
		//=========================================================================

	public:
		void mailbox_from_bitboard(piece_t board[64]) const;
		static void mailbox_from_bitboard(piece_t board[64], const bb bb);

		string debug_board() const;

		void debug_position() const;		
		static bb between[BETWEEN_ARRAY_SIZE];
		inline static uint16_t calc_index(const uint8_t& x, const uint8_t& y) {
			return x + 64 * y;
		}

		//TODO: move all the static stuff out of the class
		static string print_bitboard(bb my_bb);
		static Position create_start_position();
		static Position create_position(const string& fen);
		void print(ostream& stream) const;
		static void visualize_bitboard(bb my_bb, ostream& stream);
		static void visualize_mailbox_board(const piece_t board[64], ostream& stream);
		static void visit_mailbox_board(const int board[64], void (*visitor)(int)); // TODO convert to c++11
		string print_board() const;
		friend ostream& operator<<(ostream& stream, const Position& position);
	private:
		static string extract_row_string(uint_fast8_t row, string set);
		static void display_all_moves(const bitboard_set& moves);



		static string mailbox_board_simple_representation(const piece_t board[64]);
		static string mailbox_board_debug_representation(const piece_t board[64]);



	};
	//TODO template?
	constexpr bool is_in_back_rank_black(square_t square) {
		return Squares::A8 <= square && square <= Squares::H8;
	}
	constexpr bool is_in_back_rank_white(square_t square) {
		return Squares::A1 <= square && square <= Squares::H1;
	}

}
#endif /* POSITION_H_ */
