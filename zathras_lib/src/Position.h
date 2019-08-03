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


namespace Positions {

	//TODO find better way to do this
#define pawns  pos_bb[0]
#define knights pos_bb[1]
#define bishops pos_bb[2]
#define rooks pos_bb[3]
#define queens pos_bb[4]
#define kings pos_bb[5]
#define white pos_bb[6]
#define black pos_bb[7]


	namespace {
		const int BETWEEN_ARRAY_SIZE = 64 * 64;
	}

	using namespace Moves;
	using Move = int;//TODO
	class Position {

	public:
		Position();
		virtual ~Position();


		void make_move(const Move& move, Move_state& move_state); // TODO move semantics?

		void unmake_move(const Move& move, const Move_state& move_state);

		bool is_in_check(const bool& side);
		inline constexpr piece_t get_piece_on(const square_t& sq) {
			return board[sq];
		}


		// TODO too lazy to unify this. it may be fine just to leave the member public; I don't foresee changing the representation any time soon
		bool is_white_to_move() const {
			return white_to_move;
		}


		//TODO public for now
		bb pos_bb[8]{ 0 };//TODO use std::array
		piece_t board[64]{ 0 }; //TODO use std::array
		bb en_passant_square = 0x00;
		bitset<4> castling;
		bool white_to_move = true;
		////TODO find better way to do this
		//bb& pawns = pos_bb[0];
		//bb& knights = pos_bb[1];
		//bb& bishops = pos_bb[2];
		//bb& rooks = pos_bb[3];
		//bb& queens = pos_bb[4];
		//bb& kings = pos_bb[5];
		//bb& white = pos_bb[6];
		//bb& black = pos_bb[7];


	private:
		template<bool white_or_not> void make_move_for_colour(const square_t& from, const square_t& to, const int8_t& moving, const Move& move, Move_state& move_state, bool& set_en_passant);

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

		void debug_position();
		static bb between[BETWEEN_ARRAY_SIZE];
		inline static uint16_t calc_index(const uint8_t& x, const uint8_t& y) {
			return x + 64 * y;
		}

		//TODO: move all the static stuff out of the class
		static string print_bitboard(bb my_bb);
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
	public:
		//TODO: move all the static stuff out of the class
		static bool is_attacked_by_hopper(const bb& movers, const bitboard_set& all_moves, const square_t& square);
		static bool is_check_from_slider(const bitboard_set& sliding_moves, const square_t& king_pos, const bb& slider, const bb& occupied);

		static bool is_attacked_by_slider(bb position, const bitboard_set& all_moves, const square_t& square, const bb& occupied);
		static bool is_anything_between(square_t from, square_t to, const bb& occupied);





	};
	 void write_position(Position& position, const string& fen);
	 void write_start_position(Position& position);
	//TODO template?
	constexpr bool is_in_back_rank_black(square_t square) {
		return Squares::A8 <= square && square <= Squares::H8;
	}
	constexpr bool is_in_back_rank_white(square_t square) {
		return Squares::A1 <= square && square <= Squares::H1;
	}

}
#endif /* POSITION_H_ */
