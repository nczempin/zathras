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

#define pawns piece_bb[0]
#define knights piece_bb[1]
#define bishops piece_bb[2]
#define rooks piece_bb[3]
#define queens piece_bb[4]
#define kings piece_bb[5]

using namespace std;
namespace {
	const int BETWEEN_ARRAY_SIZE = 64*64;//2017;
}

class Position {
	static const bitboard_set rook_moves;

public:
	Position();
	virtual ~Position();

	void make_move(const Move& move, Move_state &move_state);
	void unmake_move(const Move& move, Move_state &move_state);

	bool is_in_check(const bool side);
	static bool is_attacked_by_hopper(const bb &movers,
			const bitboard_set& all_moves, const uint8_t &square);
	static bool is_check_from_slider(const bitboard_set& sliding_moves,
			const uint8_t &king_pos, const bb &slider, const bb& occupied);

	static bool is_attacked_by_slider(bb position,
			const bitboard_set& all_moves, const uint8_t &square,
			const bb &occupied);
	static bool is_anything_between(uint8_t from, uint8_t to, const bb& occupied);



	friend ostream& operator<<(ostream& stream, const Position& position);
	void print(ostream& stream) const;
	string print_board() const;
	static Position create_start_position();

	//TODO move; probably to Square class
	static void set_square(bitset<64>& bs, uint8_t to);
	static void clear_square(bitset<64>& bs, uint8_t to);
	static void set_square(bb& bs, uint8_t to);
	static bool is_set_square(bb bs, uint8_t to);
	static void clear_square(bb& bs, uint8_t to);
	static void set_bit(bb& bs, uint8_t to);
	static void clear_bit(bb& bs, uint8_t to);
	static void set_square(const uint8_t & file, const uint8_t & rank, bb & bbs);
	static void clear_square(const uint8_t & file, const uint8_t & rank, bb & bbs);
	static string print_mailbox_board(int board[64]);

	static void visualize_bitboard(bb my_bb, ostream & stream);
	static string print_bitboard(bb my_bb);
	static void visualize_mailbox_board(int board[64], ostream& stream);
	static void visit_mailbox_board(int board[64], void (*visitor)(int)); // TODO convert to c++11

	static Position create_position(const string& fen);
	//TODO a separate Bitboard (helper) class is probably best
	array<bb, 9> getPieceBitboards() const;

	bool is_white_to_move() const {
		return white_to_move;
	}

	void makeMove(string moveString) {
		Move m = Move(moveString);
		int board[64];
		for (int i = 0; i < 64; ++i) {
			board[i] = 0;
		}
		mailbox_from_bitboard(board);

		//TODO make this more elegant
		int from = m.get_from();
		uint8_t moving = board[from];
		if (moving > 6) {
			moving = -(moving - 6);
		}
		visualize_mailbox_board(board, cout);
		m.set_moving_piece(moving);
		int to = m.get_to();
		//cout << "to: " << to << endl;
		int captured = board[to];
		cout << "captured: " << captured << endl;
		if (captured > 6) {
			captured = -(captured - 6);
		}
		cout << "captured: " << captured << endl;
		m.set_captured(captured);
		//cout << "from: " << from << endl;
		//cout << "moving piece: " << moving << endl;
		//cout << "makeMove: " << moveString << endl;
		Move_state ms;
		make_move(m,ms);// m.from, m.to, m.captured, m.promoted);
		//isGivingCheck = null;
		//isReceivingCheck = null;
		print(cout);
	}
	//
	//TODO public for now
	bb piece_bb[6] = {};
	bb white = 0x00;
	bb black = 0x00;
	bb en_passant_square = 0x00;
	bitset<4> castling;
	bool white_to_move = true; //TODO public for now

	void mailbox_from_bitboard(int board[64]) const;

	void debugPosition();
private:

	static string extract_row_string(uint_fast8_t row, string set);
	static void display_all_moves(const bitboard_set& moves);
	void update_bits(bb & colour, bb & piece, uint8_t clear, uint8_t set);
	void save_en_passant_square( Move_state &move_state);
	void restore_en_passant_square(Move_state &move_state);
	void promote(int8_t promoted_to, uint8_t to);
	void un_promote(int8_t promoted_to, uint8_t to);
	void handleCapture(const uint8_t& to, const int8_t& taken, Move_state &move_state);
	

public:
	static bb between[BETWEEN_ARRAY_SIZE];
	inline static uint16_t calc_index(const uint8_t &x, const uint8_t &y) {
		return x + 64 * y;
		//return x>y ? ind(x,y):ind(y,x);
	}
//	inline static uint16_t ind(const uint8_t& x, const uint8_t& y) {
//		uint16_t retval = x + ((125 -y)* y )/2;
//		return retval;
//	}


};

#endif /* POSITION_H_ */
