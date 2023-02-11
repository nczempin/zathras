/*
 * Square.h
 *
 *  Created on: 22.04.2016
 *      Author: nczempin
 */

#ifndef SQUARE_H_
#define SQUARE_H_

#include <cstdint>
#include <string>

#include <bitset>
#include <array>


namespace Positions {
	enum Squares : int {
		A1, B1, C1, D1, E1, F1, G1, H1,
		A2, B2, C2, D2, E2, F2, G2, H2,
		A3, B3, C3, D3, E3, F3, G3, H3,
		A4, B4, C4, D4, E4, F4, G4, H4,
		A5, B5, C5, D5, E5, F5, G5, H5,
		A6, B6, C6, D6, E6, F6, G6, H6,
		A7, B7, C7, D7, E7, F7, G7, H7,
		A8, B8, C8, D8, E8, F8, G8, H8


	};

	using square_t = Squares;
	using std::bitset;
	using bb = uint_fast64_t;

	class Square
	{
	public:
		Square();
		static void init_squares();
		virtual ~Square();
		static void print_square(uint8_t square);
		static std::string mailbox_index_to_square(uint8_t x);

		static void set_square(bitset<64> & bs, square_t to);
		static void set_square(bitset<64> & bs, const uint8_t& file, const uint8_t& rank);
		static void set_square(const uint8_t& file, const uint8_t& rank, bitset<64> & bs);
		static void set_square(bb& bs, square_t to);
		static void set_square(const uint8_t& file, const uint8_t& rank, bb& bbs);

		static void clear_square(bb& bs, square_t to);
		static void clear_square(bitset<64> & bs, square_t to);
		static void clear_square(const uint8_t& file, const uint8_t& rank, bb& bbs);
		static void clear_square(const uint8_t& file, const uint8_t& rank, bitset<64> & bs);

		static bool is_set_square(bb bs, square_t to);
		static void set_bit(bb& bs, const square_t& to);
		static void preset_bit(bb& bs, const square_t& to);
		static void clear_bit(bb& bs, const square_t& to);

		static void update_bits(bb& colour, bb& piece, square_t clear, square_t set);

		static std::array<bb, 64> squares;


	};

	void toggle_square(bb& b, const square_t& from, const square_t& to);

}
#endif /* SQUARE_H_ */
