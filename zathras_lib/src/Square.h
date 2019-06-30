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
namespace Positions {
	class Square
	{
	public:
		Square();
		virtual ~Square();
		static void print_square(uint8_t square);
		static std::string mailbox_index_to_square(uint8_t x);
		enum Identifiers {
			A1, B1, C1, D1, E1, F1, G1, H1,
			A2,
			A3,
			A4,
			A8 = 56, B8, C8, D8, E8, F8, G8, H8

			
		};
		//static const int E1 = 4;
		//static const int E8 = 64 - 4;

	};
}
#endif /* SQUARE_H_ */
