#pragma once
#include <string>
namespace Util {
	int decodeSquare(std::string square);
	int decodePiece(std::string promotedTo);
	bool timeUp();
	void resetClock();
}