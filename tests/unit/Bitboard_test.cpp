/*
 * Bitboard_test.cpp
 *
 *  Created on: 24.11.2017
 *      Author: nczempin
 */
#include <iostream>

#include "Bitboard.h"
#include "typedefs.h"

void test_bitboard_ffs() {
	using namespace positions;
	for (int i = 0; i < 64; ++i) {
		int x = Bitboard::ffs(i);
		std::cout << i << ": " << x << "\n";
	}

	//	const bb& my_bb = Bitboard::BB_FULL_BOARD;
	//	const uint8_t extracted = Bitboard::extract_square(my_bb);
	//	std::cout << extracted << "\n";
}
