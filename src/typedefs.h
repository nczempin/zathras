/*
 * typedefs.h
 *
 *  Created on: 20.04.2016
 *      Author: admin
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <cstdint>
#include <vector>
#include <functional>

using namespace std;

using bb = uint_fast64_t;

using bitboard_set = vector<bb>;
using square_visitor = function<void(uint8_t)>;
using move_visitor = function<void(int8_t, uint8_t, uint8_t, int8_t)>;

#endif /* TYPEDEFS_H_ */
