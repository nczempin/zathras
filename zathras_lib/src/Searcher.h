#pragma once
#include <deque>
#include <chrono>
#include <climits>
#include <cstdint>
#include <vector>

#include "Move.h"
#include "Position.h"
#include "Move_generator.h"

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
	int idDepth = 1;
	int maxIdDepth = 64;
	long long softTimeLimitMs = 0;
	long long hardTimeLimitMs = 0;
	long long nodes = 0;
	int seldepth = 0;
	std::chrono::steady_clock::time_point startTime;
	std::vector<uint64_t> gameHistory;
	std::vector<uint64_t> searchPath;
	bool kingCapture = false;
	deque<Move> pv;

	static uint64_t hashPosition(const Position& pos);
	void clearHistory();
	void pushHistory(const Position& pos);
	void startTimer(long long softMs, long long hardMs, int maxDepth);
	long long elapsedMs() const;
	bool isHardTimeUp() const;
	bool isSoftTimeUp() const;
	
	Move findBestmove(const Move_container& move_container, Position p);

	int alphabeta(int depth, Position& position, int alpha, int beta, deque<Move>& lineUp);

	int quiescence_alphabeta(int depth, Position& position, int alpha, int beta, deque<Move>& lineUp);

	Move analyze(Position p) {
		Move_generator mg;
		if (done) {
			return Move();
		}
		const Move_container move_container = mg.generate_legal_moves(p, 1);
		const auto& moves = move_container.get_moves_ref();
		if (move_container.size() == 0) {
			return Move();
		}
		else if (move_container.size() == 1) {
			return moves[0];
		}
		else {
			Move m = findBestmove(move_container, p);
			return m;
		}
	}

};

