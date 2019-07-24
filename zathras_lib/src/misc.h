#pragma once
#include <string>

namespace Util {

	int decodeSquare(std::string square);
	int decodePiece(std::string promotedTo);
	bool timeUp();

	bool is_digit(const char c);

}