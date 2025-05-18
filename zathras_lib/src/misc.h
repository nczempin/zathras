#pragma once
#include <string>
#include <sstream>
#include <vector>

namespace Util {

	using namespace std;
	int decode_square(string square);
	int decode_piece(string promotedTo);
	//bool time_up();

	bool is_digit(const char c);

	vector<string>& split(const string& s, char delim, vector<string>& elems); 

	vector<string> split(const string& s, char delim);


}
