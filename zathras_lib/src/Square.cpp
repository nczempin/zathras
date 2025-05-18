/*
 * Square.cpp
 *
 *  Created on: 22.04.2016
 *      Author: nczempin
 */

#include <iostream>
#include <cassert>
#include <bitset>

#include "Square.h"
#include "typedefs.h"

namespace positions {
	using namespace std;

	Square::Square()
	{
	}
	void Square::init_squares() {

		for (int i = 0; i < 64; ++i) {
			squares[i] = 0;
			Square::preset_bit(squares[i], square_t(i));
		}
	}
	array<bb, 64> Square::squares;

	Square::~Square()
	{
		// TODO Auto-generated destructor stub
	}

	void Square::print_square(uint8_t x)
	{
		string square = mailbox_index_to_square(x);
		cout << x << " = " << square << endl;

	}

	string Square::mailbox_index_to_square(uint8_t x)
	{
		const char column = 'a' + x % 8;
		string columnString(1, column);
		const char row = '1' + x / 8;
		string rowString(1, row);
		string square = columnString + rowString;
		return square;
	}


	void Square::set_square(bitset<64> & bs, const square_t to) {
		bs[to] = true;
	}

	void Square::set_square(bb& b, square_t to) {
		bb tmp = 1ULL << to;
		b |= tmp;
	}
	void Square::set_square(const uint8_t& file, const uint8_t& rank, bb& bbs) {
		//uint8_t to_twisted = 7 - file + rank * 8;
		const uint8_t to = file + rank * 8;
		Square::set_square(bbs, square_t(to));
	}
	void Square::set_square(const uint8_t& file, const uint8_t& rank, bitset<64> & bs) {
		Square::set_square(bs, file, rank);
	}
	void Square::set_square(bitset<64> & bs, const uint8_t& file, const uint8_t& rank) {
		//uint8_t to_twisted = 7 - file + rank * 8;
		const uint8_t to = file + rank * 8;
		Square::set_square(bs, square_t(to));
	}


	void Square::clear_square(bitset<64> & bs, square_t to) {
		bs[to] = false;
	}
	void Square::clear_square(bb& b, square_t to) {
		bb tmp = ~(1ULL << to);
		b &= tmp;
	}
	void Square::clear_square(const uint8_t& file, const uint8_t& rank, bb& bbs) {
		//uint8_t to_twisted = 7 - file + rank * 8;
		uint8_t to = file + rank * 8;
		Square::clear_square(bbs, square_t(to));
	}
	void Square::clear_square(const uint8_t& file, const uint8_t& rank, bitset<64> & bs) {
		const uint8_t to = file + rank * 8;
		Square::clear_square(bs, square_t(to));
	}





	void Square::set_bit(bb& b, const square_t& to) {
		Square::set_square(b, square_t(to));
		//b |= squares[to];
	}
	void Square::preset_bit(bb& b, const square_t& to) {
		Square::set_square(b, square_t(to));
		//uint8_t rank = to / 8;
		//uint8_t file = (to % 8);
		//set_square(file, rank, b);
	}
	void Square::clear_bit(bb& b, const square_t& to) {

		assert(to < 64);
		clear_square(b, to);
		//b &= ~squares[to];
	}
	bool Square::is_set_square(bb b, square_t to) {
		return b & squares[to];

	}




	void Square::update_bits(bb& colour, bb& piece, square_t clear, square_t set) { //TODO castling rights on regular rook move

		set_bit(piece, set);
		set_bit(colour, set);
		clear_bit(piece, clear);
		clear_bit(colour, clear);
	}

	void toggle_square(bb& b, const square_t& from, const square_t& to) {
		const bb tmp = (1ULL << from) ^ (1ULL << to);
		b ^= tmp;
	}

}

