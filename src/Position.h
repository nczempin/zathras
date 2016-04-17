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

typedef uint64_t  bb;

typedef vector<bb> bitboard_set;
typedef function<void(int)> square_visitor;

class Position {
public:
	Position();
	virtual ~Position();

	friend ostream& operator<<(ostream& stream, const Position& position);
	void print(ostream& stream) const;
	static void setSquare(bitset<64>& bs, int to);
	static void clearSquare(bitset<64>& bs, int to);
	static void visualize_bitboard(bb my_bb, ostream& stream);
	static void visit_bitboard(bb my_bb, square_visitor);
	static void visualize_mailbox_board(int board[64], ostream& stream);
	static void visit_mailbox_board(int board[64], void (*visitor)(int)); // TODO convert to c++11
	static void print_square(int x);
	static string mailboxIndexToSquare(int x);
	static const bb BB_FULL_BOARD = 		0xffffffffffffffff;
	static const bb BB_RANK8 = 			0xff00000000000000;
	static const bb BB_RANK3 = 		0x0000000000ff0000;
	static const bb BB_RANK4 = 		0x00000000ff000000;
	static const bb BB_RANK5 = 		0x000000ff00000000;
	static const bb BB_RANK6 = 		0x0000ff0000000000;
	static const bb BB_RANK3N6 = 	BB_RANK3 | BB_RANK6;
  static shared_ptr<Position> create_start_position();
  bitboard_set getPieceBitboards();
private:
	bool white_to_move = true;
	bb pawns;
	bb knights;
	bb bishops;
	bb rooks;
	bb queens;
	bb kings;
	bb white;
	bb black;

	static string extract_row_string(uint_fast8_t row, string set);
	static void display_all_moves(const bitboard_set& moves);
};

#endif /* POSITION_H_ */
