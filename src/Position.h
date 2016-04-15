/*
 * Position.h
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#ifndef POSITION_H_
#define POSITION_H_

#include <bitset>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

typedef uint_fast64_t bb;

typedef vector<uint_fast64_t> bitboard_set;


class Position {
public:
	Position();
	virtual ~Position();
	static shared_ptr<Position> create_start_position();
	friend ostream& operator<<(ostream& stream, const Position& position);
	void print(ostream& stream) const;
	void pregenerate_moves();
private:
	bool white_to_move = true;
	bb everything = 0xffffffffffffffff;
	bb top_row = 0xff00000000000000;

	// test position
	bb pawns = 0x008469040000e700;
	bb knights = 0x4000000000000000;
	bb bishops = 0x0000100000100000;
	bb rooks = 0x0100000000000009;
	bb queens = 0x8000000010000000;
	bb kings = 0x0800000000000800;
	bb white = 0x800000000010ef09;
	bb black = 0x4984790410000000;
	// end of test position

	static string extract_row_string(uint_fast8_t row, string set);
	static void visualize_bitboard(bb my_bb, ostream& stream);
	static void visit_bitboard(bb my_bb, function<void(int)>);
	static void visualize_mailbox_board(int board[64], ostream& stream);
	static void visit_mailbox_board(int board[64], void (*visitor)(int));
	static pair<bitboard_set, bitboard_set> pregenerate_hoppers(vector<int> jumps);
	static pair<bitboard_set, bitboard_set> pregenerate_rays(int direction);
	static pair<bitboard_set, bitboard_set> pregenerate_knight_moves();
	static pair<bitboard_set, bitboard_set> pregenerate_king_moves();
	static pair<bitboard_set, bitboard_set> pregenerate_bishop_moves();
	static pair<bitboard_set, bitboard_set> pregenerate_rook_moves();
	static pair<bitboard_set, bitboard_set> pregenerate_queen_moves();
	static bitboard_set pregenerate_white_pawn_no_capture_moves();
	static bitboard_set pregenerate_black_pawn_no_capture_moves();
	static bitboard_set pregen_pawn_nocaps(int start, int stop, int direction);
	static pair<bitboard_set, bitboard_set> pregenerate_white_pawn_capture_moves();
	static pair<bitboard_set, bitboard_set> pregenerate_black_pawn_capture_moves();
	static pair<bitboard_set, bitboard_set> pregen_pawn_caps(int direction);
	static void place_pawn_move(int from, int steps, int direction, bitset<64> bs[64]);
	static void print_square(int x);
	static void setSquare(bitset<64>& bs, int to);
	static void clearSquare(bitset<64>& bs, int to);
	void display_all_moves(const pair<bitboard_set, bitboard_set>& moves);
};

#endif /* POSITION_H_ */
