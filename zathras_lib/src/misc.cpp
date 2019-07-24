#include "misc.h"

namespace Util {
	using namespace std;


	int decodeSquare(string square) {
		char letter = square[0];
		char digit = square[1];
		int one = letter - 'a';
		int ten = digit - '1';
		int index = 8 * ten + one;
		return index;
	}



	int decodePiece(string promotedTo) {
		int retValue = 0;
		if (promotedTo == "q") {
			retValue = 5;
		}
		else if (promotedTo == "r") {
			retValue = 4;
		}
		else if (promotedTo == "b") {
			retValue = 3;
		}
		else if (promotedTo == "n")
			retValue = 2;
		return retValue;
	}

	bool is_digit(const char c) {
		return '0' <= c && c <= '9';
	}
}