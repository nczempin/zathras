#include "misc.h"
#include <vector>

namespace Util {
	using namespace std;


	int decode_square(string square) {
		char letter = square[0];
		char digit = square[1];
		int one = letter - 'a';
		int ten = digit - '1';
		int index = 8 * ten + one;
		return index;
	}



	int decode_piece(string promotedTo) {
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

	vector<string>& split(const string& s, char delim, vector<string>& elems) {
		//TODO why am I both passing and returning elems? copy in between?
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	vector<string> split(const string& s, char delim) {
		vector<string> elems;
		split(s, delim, elems);
		return elems;
	}
}
