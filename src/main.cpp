

#include <iostream>
#include <string>



#include "Uci.h"

using namespace std;
const string VERSION = "0.0.3";

//TODO deal with these globals
Position p;
Searcher searcher;
future<Move> fut;

int main()
{
	cout << "Welcome to Zathras, a chess engine by Nicolai Czempin. This is version " << VERSION << endl;
	string mystr;
	while (true) {
		getline(cin, mystr);
		Interface::Uci::parse(mystr);
	}
	return 0;
}
