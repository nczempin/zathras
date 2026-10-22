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

	struct TTEntry {
		uint64_t key = 0;
		int score = 0;
		int16_t depth = -1;
		uint8_t flag = 0; // 0=none, 1=exact, 2=lowerbound, 3=upperbound
		Move bestMove;
	};
	static constexpr size_t TT_SIZE = 65536;
	std::vector<TTEntry> tt{TT_SIZE};
	Move killers[64][2];
	int historyTable[64][64]{};

	static uint64_t hashPosition(const Position& pos);
	void clearHistory();
	void pushHistory(const Position& pos);
	void startTimer(long long softMs, long long hardMs, int maxDepth);
	long long elapsedMs() const;
	bool isHardTimeUp() const;
	bool isSoftTimeUp() const;
	
	Move findBestmove(const Move_container& move_container, Position p);

	int alphabeta(int depth, Position& position, int alpha, int beta, deque<Move>& lineUp);

	int quiescence_alphabeta(int depth, Position& position, int alpha, int beta);

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

