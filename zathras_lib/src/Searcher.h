#pragma once
#include <atomic>
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
	std::atomic<bool> done{false};
	int idDepth = 1;
	int maxIdDepth = 64;
	long long softTimeLimitMs = 0;
	long long hardTimeLimitMs = 0;
	long long nodes = 0;
	int seldepth = 0;
	std::chrono::steady_clock::time_point startTime;
	std::vector<uint64_t> gameHistory;
	std::vector<uint64_t> searchPath;
	int rootLastIrreversible = 0;
	bool repetitionSeen = false;
	// PV is currently tracked via std::deque<Move> rather than a stack-allocated triangular PV array.
	deque<Move> pv;

	// Fixed-size always-replace TT without mate-score ply normalization or depth/age replacement.
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

	// Full-board SplitMix64 hash recomputed per node (not yet incrementally maintained in Position).
	static uint64_t hashPosition(const Position& pos);
	void clearHistory();
	void resetGameHistory();
	void pushHistory(const Position& pos, bool irreversible = false);
	void startTimer(long long softMs, long long hardMs, int maxDepth);
	long long elapsedMs() const;
	bool isHardTimeUp() const;
	bool isSoftTimeUp() const;
	
	Move findBestmove(const Move_container& move_container, Position p);

	int alphabeta(int depth, Position& position, int alpha, int beta, deque<Move>& lineUp, int lastIrreversible, int rootIndex);

	int quiescence_alphabeta(int depth, Position& position, int alpha, int beta);

	Move analyze(Position p) {
		Move_generator mg;
		const Move_container move_container = mg.generate_legal_moves(p);
		const auto& moves = move_container.get_moves_ref();
		if (move_container.size() == 0) {
			return Move();
		}
		else if (move_container.size() == 1 || done) {
			return moves[0];
		}
		else {
			Move m = findBestmove(move_container, p);
			return m;
		}
	}

};

