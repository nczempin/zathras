/*
 * typedefs.h
 *
 *  Created on: 20.04.2016
 *      Author: nczempin
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <cstdint>
#include <vector>
#include <array>
#include <functional>

using namespace std;

using bb = uint_fast64_t;

using bitboard_set = array<bb, 64>; // one bitboard for every starting square
using square_visitor = function<void(uint8_t)>;
using move_visitor = function<void(int8_t moving, uint8_t from, uint8_t to, int8_t captured, int8_t promoted_to)>;

using piece_t = int8_t;
using square_t = uint8_t;

#endif /* TYPEDEFS_H_ */
