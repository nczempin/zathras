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

typedef uint_fast64_t bb;

typedef vector<bb> bitboard_set;
typedef function<void(int)> square_visitor;
typedef function<void(int8_t, uint8_t, uint8_t, int8_t)> move_visitor;

#endif /* TYPEDEFS_H_ */
