#pragma once
#include <deque>
#include <chrono>


#include "Move.h"
#include <climits>
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
	
	Move findBestmove(move_container_t moves, Position p);

	int alphabeta(int depth, Position& position, int alpha, int beta, deque<Move>& lineUp);

	int quiescence_alphabeta(int depth, Position& position, int alpha, int beta, deque<Move>& lineUp);

	Move analyze(Position p) {
		Move_generator mg;
		if (done) {
			return Move();
		}
		const Move_container move_container = mg.generate_legal_moves(p, 1);
		auto moves = move_container.get_moves();
		if (move_container.size() == 0) {
			return Move();// -1, -1, 0); //TODO
		}
		else if (move_container.size() == 1) {
			auto move = moves[0];
			cout << to_string(move) << endl;
			return move;
		}
		else {
			Move m = findBestmove(moves, p);
			return m;
		}
	}

};

