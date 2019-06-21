#pragma once
#include <deque>
#include <chrono>

#include "global.h"
#include "Move.h"
#include "Position.h"
#include "Move_generator.h"
#include "Info.h"

using namespace std;

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

		Move_container move_container = mg.generate_legal_moves(p, 1);
		auto moves = move_container.get_moves();
		if (move_container.size() == 0) {
			return nullptr;// Move(-1, -1, 0); //TODO
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


	void updateNps() {
		chrono::duration<double> elapsed_seconds = chrono::system_clock::now() - Info::start;
		double seconds = elapsed_seconds.count();
		Info::nps = (Info::nodes / seconds);
	}

	void printInfo() {
		cout << "info depth " << idDepth;
		cout << " seldepth " << Info::seldepth;
		cout << " currmove " << Info::currmove.to_string();
		cout << " currmovenumber " << Info::currmovenumber;
		cout << " nodes " << Info::nodes;
		updateNps();
		cout << " nps " << Info::nps;
		cout << " score ";
		if (bestValue > 80000) {
			cout << "mate " << idDepth / 2;
		}
		else if (bestValue < 900000) {
			cout << "cp " << oldBestValue;
		}
		else {
			cout << "cp " << bestValue;
		}
		cout << " pv ";
		for (Move m : pv) {
			cout << m.to_string() << " ";
		}
		cout << endl;
	}
};

