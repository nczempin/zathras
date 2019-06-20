#include "global.h"
//  main.cpp


#include <iostream>
#include <chrono>
#include <future>
#include <thread>

#include "global.h"
#include "Position.h"
#include "Move_generator.h"
#include "Searcher.h"
//#include "Evaluator.h"

#include "Uci.h"

using namespace std;

Position p;
Searcher searcher;

long timePerMove;

//static string extractPosition(string);
//static string extractMoves(string);
future<Move> fut;
//vector<string>& split(const string& s, char delim, vector<string>& elems) {
//	stringstream ss(s);
//	string item;
//	while (getline(ss, item, delim)) {
//		elems.push_back(item);
//	}
//	return elems;
//}
//
//vector<string> split(const string& s, char delim) {
//	vector<string> elems;
//	split(s, delim, elems);
//	return elems;
//}

bool convertColor(int piece) {
	return piece > 0;
}
int decodeSquare(string square) {
	char letter = square[0];
	char digit = square[1];
	int one = letter - 'a';
	int ten = digit - '1' ;
	int index = 8 * ten + one;
	return index;
}

string encodeSquare(int square) {
	int ten = square / 10;
	int one = square - ten * 10;
	char letter = (int)'a' + one - 1;
	char number = (int)'1' + ten - 1;
	string retVal;
	retVal += letter;
	retVal += number;

	return retVal;
}
long calculateTimePerMove(long t, long inc, int movesToGo) {
	if ((t == 0L) && (inc == 0L))
		return -1L; //TODO Exception?
	int slice = movesToGo + 1;
	long retVal = (t - inc) / slice + inc - 500L;
	if (retVal < 100L)
		retVal = 100L;
	return retVal;
}

void resetClock() {
	Info::start = chrono::system_clock::now();
}

bool timeUp()
{
	if (timePerMove <= 0L)
		return false;
	//return false;
	auto now = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = now - Info::start;
	return elapsed_seconds.count() >= timePerMove / 1000.0;
}




static long perft(const Position position, int maxDepth) {
	if (maxDepth == 0) {
		return 1;
	}
	Move_generator mg;
	Move_container move_container = mg.generate_legal_moves(position,1);
	int count = 0;
	Position tmpPos = position;
	Move_state ms;
	for (Move move : move_container.get_moves()) {
		if (move.get_moving_piece() == 0) {
			break;
		}
		tmpPos = position;
		tmpPos.make_move(move, ms);
		count += perft(tmpPos, maxDepth - 1);
	}

	return count;
}

bool invalidSquare(int next) {
	bool isInvalid = false;
	if ((next < 11) || (next > 88))
		isInvalid = true;
	int mod = next % 10;
	if ((mod == 0) || (mod == 9))
		isInvalid = true;
	return isInvalid;
}


void printInfo() {
	while (!searcher.done) {
		searcher.printInfo();
		this_thread::sleep_for(chrono::seconds(1));
	}
}
int extractIntValue(string parameters, string s) {
	size_t index = parameters.find(s);
	if (index == string::npos) {
		return 0;
	}
	size_t searchFrom = index + s.length() + 1;
	string extracted = parameters.substr(searchFrom);
	string first = split(extracted, ' ')[0];
	return stoi(extracted);
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

int main()
{
	cout << "Welcome to Zathras, a chess engine by Nicolai Czempin. This is version " << VERSION << endl;
	string mystr;
	while (true) {
		getline(cin, mystr);
		Uci::parse(mystr);
	}
	return 0;
}
