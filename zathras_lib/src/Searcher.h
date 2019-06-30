#pragma once
#include <deque>
#include <chrono>


#include "Move.h"
#include "Position.h"
#include "Move_generator.h"
//#include "Info.h"

using namespace Moves;
class Searcher
{
public:
	Searcher(void);
	~Searcher(void);
	Move bestMove;
	int bestValue = INT_MIN;
	int oldBestValue = INT_MIN;
	bool done = false;
	int idDepth = -1; //TODO find good init value
	bool kingCapture = false;
	deque<Move> pv;
	
	Move findBestmove(array<Move, Move_container::SIZE> moves, Position p);

	int alphabeta(int depth, Position& position, int alpha, int beta, deque<Move>& lineUp);

	int quiescence_alphabeta(int depth, Position& position, int alpha, int beta, deque<Move>& lineUp);

	Move analyze(Position p) {
		Move_generator mg;

		const Move_container move_container = mg.generate_legal_moves(p, 1);
		auto moves = move_container.get_moves();
		if (move_container.size() == 0) {
			return Move();// -1, -1, 0); //TODO
		}
		else if (move_container.size() == 1) {
			auto move = moves[0];
			cout << move.to_string() << endl;
			return move;
		}
		else {
			Move m = findBestmove(moves, p);
			return m;
		}
	}


	/*void updateNps() {
		Interface::Info::updateNps();
	}

	*/
};

