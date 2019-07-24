

#include <iostream>
#include <string>


#include "zathras_lib.h"
#include "Uci.h"


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
	/*const bitboard_set Bitboard::king_moves = Move_generator::pregenerate_king_moves();
	const bitboard_set Bitboard::bishop_moves = Move_generator::pregenerate_bishop_moves();
	const bitboard_set Bitboard::rook_moves = Move_generator::pregenerate_rook_moves();
	const bitboard_set Bitboard::queen_moves = Move_generator::pregenerate_queen_moves();
	const bitboard_set Bitboard::white_pawn_no_capture_moves = Move_generator::pregenerate_white_pawn_no_capture_moves();
	const bitboard_set Bitboard::black_pawn_no_capture_moves = Move_generator::pregenerate_black_pawn_no_capture_moves();
	const bitboard_set Bitboard::white_pawn_capture_moves = Move_generator::pregenerate_white_pawn_capture_moves();
	const bitboard_set Bitboard::black_pawn_capture_moves = Move_generator::pregenerate_black_pawn_capture_moves();*/


	while (true) {
		getline(cin, mystr);
		Interface::Uci::parse(mystr);
	}
	return 0;
}
