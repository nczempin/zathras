

#include <iostream>
#include <string>


#include "zathras_lib.h"
#include "Uci.h"
using namespace Interface;


using namespace std;

//TODO deal with these globals
Position p;
Searcher searcher;
future<Move> fut;
extern const std::string VERSION;

int main()
{
	cout << "Welcome to Zathras, a chess engine by Nicolai Czempin. This is version " << VERSION << endl;

	string mystr;
	Zathras_lib::init();
	/*Positions::Position::visualize_bitboard(Bitboard::bishop_moves[0], cout);
	Positions::Position::visualize_bitboard(Bitboard::bishop_moves[4], cout);
	Positions::Position::visualize_bitboard(Bitboard::bishop_moves[7], cout);*/
	
	write_start_position(p);
	//std::this_thread::sleep_for(std::chrono::milliseconds(4000));

	//Perft_command pc{ p,  6 };
	//pc.execute();


	while (true) {
		getline(cin, mystr);
		Interface::Uci::parse(mystr);
	}
	return 0;
}
