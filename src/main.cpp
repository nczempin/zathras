#include <iostream>
#include <string>


#include "zathras_lib.h"
#include "Uci.h"


using namespace std;

//TODO deal with these globals
Position p;
Searcher searcher;
future<Move> fut;

int main()
{
	cout << "Welcome to Zathras, a chess engine by Nicolai Czempin. This is version " << Zathras_lib::VERSION << endl;

	Zathras_lib::init();
	
	while (true) {
		string command;
		getline(cin, command);
		zathras::interface::Uci::parse(command);
	}
	return 0;
}
