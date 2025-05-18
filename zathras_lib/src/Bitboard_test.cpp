/*
 * Bitboard_test.cpp
 *
 *  Created on: 24.11.2017
 *      Author: nczempin
 */
#include <iostream>

#include "bitboard.h"
#include "typedefs.h"

namespace positions {
	int main_() {
		for (int i = 0; i < 64; ++i) {
			int x = Bitboard::ffs(i);
			cout << i << ": " << x << "\n";
		}

		//	const bb& my_bb = Bitboard::BB_FULL_BOARD;
		//	const uint8_t extracted = Bitboard::extract_square(my_bb);
		//	std::cout << extracted << "\n";
		return 0;
	}

}
