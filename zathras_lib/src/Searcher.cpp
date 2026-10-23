#include <vector>
#include <algorithm>
#include <set>
#include <iostream>
#include <cassert>
#include <cstdlib>

#include "Searcher.h"
#include "Evaluator.h"
#include "Move.h"
#include "Bitboard.h"
#include "Piece.h"

namespace {
	constexpr int INF_SCORE = 1000000;
	constexpr int MATE_SCORE = 100000;
	constexpr int MATE_THRESHOLD = 90000;
	constexpr uint8_t TT_EXACT = 1;
	constexpr uint8_t TT_ALPHA = 2;
	constexpr uint8_t TT_BETA = 3;

	inline bool sameMove(const Move& a, const Move& b) {
		return a.get_from() == b.get_from() &&
		       a.get_to() == b.get_to() &&
		       a.get_move_type() == b.get_move_type();
	}

	inline bool isIrreversibleMove(const Position& posBefore, const Move& m) {
		if (posBefore.get_piece_on(m.get_to()) != 0 || m.get_move_type() == EN_PASSANT || m.is_promotion()) {
			return true;
		}
		piece_t moving = posBefore.get_piece_on(m.get_from());
		return moving == Piece::WHITE_PAWN || moving == Piece::BLACK_PAWN;
	}

	inline int scoreMove(const Position& pos, const Move& m, const Move* ttMove = nullptr, const Move* k0 = nullptr, const Move* k1 = nullptr, const int (*hist)[64] = nullptr) {
		if (ttMove != nullptr && sameMove(m, *ttMove)) {
			return 100000;
		}
		int score = 0;
		piece_t captured = pos.get_piece_on(m.get_to());
		if (captured != 0) {
			int victim = std::abs(static_cast<int>(captured));
			int attacker = std::abs(static_cast<int>(pos.get_piece_on(m.get_from())));
			score += 10000 + victim * 100 - attacker;
		} else if (m.get_move_type() == EN_PASSANT) {
			score += 10090;
		} else {
			if (k0 != nullptr && sameMove(m, *k0)) {
				score += 8000;
			} else if (k1 != nullptr && sameMove(m, *k1)) {
				score += 7000;
			} else if (hist != nullptr) {
				uint8_t f = static_cast<uint8_t>(m.get_from()) & 63;
				uint8_t t = static_cast<uint8_t>(m.get_to()) & 63;
				score += std::min(6000, hist[f][t]);
			}
		}
		if (m.is_promotion()) {
			score += (m.get_move_type() == PROMOTION_QUEEN) ? 9000 : 5000;
		}
		return score;
	}

	struct PathGuard {
		std::vector<uint64_t>& path;
		~PathGuard() {
			path.pop_back();
		}
	};
}

Searcher::Searcher(void)
{
}

Searcher::~Searcher(void)
{
}

uint64_t Searcher::hashPosition(const Position& pos) {
	auto mix = [](uint64_t x) {
		x ^= x >> 30;
		x *= 0xbf58476d1ce4e5b9ULL;
		x ^= x >> 27;
		x *= 0x94d049bb133111ebULL;
		x ^= x >> 31;
		return x;
	};
	uint64_t h = pos.white_to_move ? 0x9e3779b97f4a7c15ULL : 0ULL;
	h ^= mix(pos.white ^ 0x1111111111111111ULL);
	h ^= mix(pos.black ^ 0x2222222222222222ULL);
	for (int i = 0; i < 6; ++i) {
		h ^= mix(pos.piece_bb[i] + (static_cast<uint64_t>(i + 1) << 48));
	}
	uint64_t castlingMask = (pos.castling[0] ? 1ULL : 0ULL)
		| (pos.castling[1] ? 2ULL : 0ULL)
		| (pos.castling[2] ? 4ULL : 0ULL)
		| (pos.castling[3] ? 8ULL : 0ULL);
	uint64_t epHash = 0ULL;
	if (pos.en_passant_square != 0) {
		uint8_t epSq = Bitboard::extract_square(pos.en_passant_square);
		if (epSq < 64) {
			bb attackers = pos.white_to_move
				? (Bitboard::black_pawn_capture_moves[epSq] & (pos.piece_bb[0] & pos.white))
				: (Bitboard::white_pawn_capture_moves[epSq] & (pos.piece_bb[0] & pos.black));
			if (attackers != 0) {
				epHash = pos.en_passant_square;
			}
		}
	}
	h ^= mix((castlingMask + 0x10ULL) ^ (epHash * 0x517cc1b727220a95ULL));
	return h;
}

void Searcher::resetGameHistory() {
	gameHistory.clear();
	rootLastIrreversible = 0;
}

void Searcher::clearHistory() {
	resetGameHistory();
	std::fill(tt.begin(), tt.end(), TTEntry{});
	for (int d = 0; d < 64; ++d) {
		killers[d][0] = Move();
		killers[d][1] = Move();
		for (int s = 0; s < 64; ++s) {
			historyTable[d][s] = 0;
		}
	}
}

void Searcher::pushHistory(const Position& pos, bool irreversible) {
	if (irreversible) {
		rootLastIrreversible = static_cast<int>(gameHistory.size());
	}
	gameHistory.push_back(hashPosition(pos));
}

void Searcher::startTimer(long long softMs, long long hardMs, int maxDepth) {
	startTime = std::chrono::steady_clock::now();
	softTimeLimitMs = softMs;
	hardTimeLimitMs = hardMs;
	maxIdDepth = (maxDepth > 0) ? maxDepth : 64;
	nodes = 0;
	seldepth = 0;
	done = false;
	for (int d = 0; d < 64; ++d) {
		killers[d][0] = Move();
		killers[d][1] = Move();
		for (int s = 0; s < 64; ++s) {
			historyTable[d][s] /= 2;
		}
	}
}

long long Searcher::elapsedMs() const {
	auto now = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
}

bool Searcher::isHardTimeUp() const {
	if (hardTimeLimitMs <= 0) return false;
	return elapsedMs() >= hardTimeLimitMs;
}

bool Searcher::isSoftTimeUp() const {
	if (softTimeLimitMs <= 0) return false;
	return elapsedMs() >= softTimeLimitMs;
}

Move Searcher::findBestmove(const Move_container& move_container, Position position) {
	int moveCount = static_cast<int>(move_container.size());
	const auto& rawMoves = move_container.get_moves_ref();
	std::vector<Move> rootMoves(rawMoves.begin(), rawMoves.begin() + moveCount);

	// Initial root ordering by MVV-LVA
	std::stable_sort(rootMoves.begin(), rootMoves.end(), [&position](const Move& a, const Move& b) {
		return scoreMove(position, a) > scoreMove(position, b);
	});

	idDepth = 1;
	bestMove = rootMoves[0];
	bestValue = -INF_SCORE;

	do {
		searchPath = gameHistory;
		int baseIrreversible = rootLastIrreversible;
		if (searchPath.empty() || searchPath.back() != hashPosition(position)) {
			searchPath.push_back(hashPosition(position));
		}
		int rootIndex = static_cast<int>(searchPath.size()) - 1;
		if (baseIrreversible > rootIndex) {
			baseIrreversible = rootIndex;
		}

		// Put previous iteration's bestMove first
		for (size_t i = 1; i < rootMoves.size(); ++i) {
			if (sameMove(rootMoves[i], bestMove)) {
				std::rotate(rootMoves.begin(), rootMoves.begin() + i, rootMoves.begin() + i + 1);
				break;
			}
		}

		int iterBestValue = -INF_SCORE;
		Move iterBestMove = rootMoves[0];
		deque<Move> iterBestPv;
		deque<Move> lineDown;
		bool completedAtLeastOneMove = false;

		for (size_t i = 0; i < rootMoves.size(); ++i) {
			const Move& move = rootMoves[i];
			bool irrev = isIrreversibleMove(position, move);
			Move_state ms;
			position.make_move(move, ms);
			++nodes;

			int childIrrev = (irrev || position.castling != ms.prev_castling)
				? static_cast<int>(searchPath.size())
				: baseIrreversible;

			lineDown.clear();
			repetitionSeen = false;
			int value;
			if (i == 0) {
				value = -alphabeta(1, position, -INF_SCORE, -iterBestValue, lineDown, childIrrev, rootIndex);
			} else {
				value = -alphabeta(1, position, -iterBestValue - 1, -iterBestValue, lineDown, childIrrev, rootIndex);
				if (value > iterBestValue && !done) {
					lineDown.clear();
					value = -alphabeta(1, position, -INF_SCORE, -iterBestValue, lineDown, childIrrev, rootIndex);
				}
			}
			position.unmake_move(move, ms);

			if (done) {
				break;
			}

			completedAtLeastOneMove = true;
			if (value > iterBestValue) {
				iterBestValue = value;
				iterBestMove = move;
				iterBestPv = lineDown;
				iterBestPv.push_front(move);
			}
		}

		if (completedAtLeastOneMove) {
			bestMove = iterBestMove;
			bestValue = iterBestValue;
			pv = iterBestPv;

			long long ms = std::max(1LL, elapsedMs());
			long long nps = (nodes * 1000LL) / ms;
			std::cout << "info depth " << idDepth
			          << " seldepth " << std::max(idDepth, seldepth);
			if (bestValue > MATE_THRESHOLD) {
				int matePly = MATE_SCORE - bestValue;
				int mateMoves = (matePly + 1) / 2;
				std::cout << " score mate " << mateMoves;
			} else if (bestValue < -MATE_THRESHOLD) {
				int matePly = MATE_SCORE + bestValue;
				int mateMoves = -((matePly + 1) / 2);
				std::cout << " score mate " << mateMoves;
			} else {
				std::cout << " score cp " << bestValue;
			}
			std::cout << " nodes " << nodes
			          << " nps " << nps
			          << " time " << ms
			          << " pv";
			for (const Move& m : pv) {
				std::cout << " " << Moves::to_string(m);
			}
			std::cout << std::endl;

			if (bestValue > MATE_THRESHOLD) {
				done = true;
			}
		}

		if (idDepth >= maxIdDepth || isSoftTimeUp() || isHardTimeUp()) {
			done = true;
		}
		++idDepth;
	} while (!done);

	return bestMove;
}

int Searcher::alphabeta(int depth, Position& position, int alpha, int beta, deque<Move>& lineUp, int lastIrreversible, int rootIndex) {
	if ((nodes & 255) == 0 && isHardTimeUp()) {
		done = true;
	}
	if (done) {
		return 0;
	}
	if (depth > seldepth) {
		seldepth = depth;
	}

	uint64_t h = hashPosition(position);
	int preRootMatches = 0;
	for (int i = static_cast<int>(searchPath.size()) - 2; i >= lastIrreversible; i -= 2) {
		if (searchPath[i] == h) {
			if (i >= rootIndex) {
				repetitionSeen = true;
				return 0; // Repetition within search tree (or returning to root)
			}
			++preRootMatches;
			if (preRootMatches >= 2) {
				repetitionSeen = true;
				return 0; // Threefold repetition with pre-root game history
			}
		}
	}

	int remDepth = idDepth - depth;
	if (remDepth <= 0) {
		return quiescence_alphabeta(depth, position, alpha, beta);
	}

	int origAlpha = alpha;
	TTEntry& tte = tt[h & (TT_SIZE - 1)];
	const Move* ttMovePtr = nullptr;
	if (tte.flag != 0 && tte.key == h) {
		ttMovePtr = &tte.bestMove;
		if (tte.depth >= remDepth && tte.score < MATE_THRESHOLD && tte.score > -MATE_THRESHOLD) {
			if (tte.flag == TT_EXACT) {
				lineUp.clear();
				lineUp.push_front(tte.bestMove);
				return tte.score;
			}
			if (tte.flag == TT_BETA && tte.score >= beta) {
				return tte.score;
			}
			if (tte.flag == TT_ALPHA && tte.score <= alpha) {
				return tte.score;
			}
		}
	}

	bool inCheck = position.is_in_check(position.is_white_to_move());

	Move_generator mg;
	Move_container mc = mg.generate_pseudolegal_moves(position);
	int count = static_cast<int>(mc.size());

	searchPath.push_back(h);
	PathGuard guard{searchPath};

	auto& moves = mc.get_moves_ref();
	int scores[CONTAINER_SIZE];
	int ply = std::min(63, depth);
	for (int i = 0; i < count; ++i) {
		scores[i] = scoreMove(position, moves[i], ttMovePtr, &killers[ply][0], &killers[ply][1], historyTable);
	}

	Move bestLocalMove;
	bool hasBestMove = false;
	int legalMovesCount = 0;
	bool savedRepetitionSeen = repetitionSeen;
	repetitionSeen = false;
	bool localRepetitionSeen = false;

	for (int i = 0; i < count; ++i) {
		int bestIdx = i;
		for (int j = i + 1; j < count; ++j) {
			if (scores[j] > scores[bestIdx]) {
				bestIdx = j;
			}
		}
		if (bestIdx != i) {
			std::swap(moves[i], moves[bestIdx]);
			std::swap(scores[i], scores[bestIdx]);
		}

		const Move& newMove = moves[i];
		bool isCapture = (position.get_piece_on(newMove.get_to()) != 0) || (newMove.get_move_type() == EN_PASSANT);
		bool isPromo = newMove.is_promotion();
		bool irrev = isIrreversibleMove(position, newMove);

		Move_state ms;
		position.make_move(newMove, ms);
		if (position.is_in_check(!position.is_white_to_move())) {
			position.unmake_move(newMove, ms);
			continue;
		}
		++legalMovesCount;
		++nodes;

		int childIrrev = (irrev || position.castling != ms.prev_castling)
			? static_cast<int>(searchPath.size())
			: lastIrreversible;

		deque<Move> lineDown;
		int value;
		if (legalMovesCount == 1) {
			value = -alphabeta(depth + 1, position, -beta, -alpha, lineDown, childIrrev, rootIndex);
		} else {
			int nextDepth = depth + 1;
			if (remDepth >= 3 && legalMovesCount > 3 && !inCheck && !isCapture && !isPromo && !position.is_in_check(position.is_white_to_move())) {
				nextDepth = depth + 2; // LMR
			}
			value = -alphabeta(nextDepth, position, -alpha - 1, -alpha, lineDown, childIrrev, rootIndex);
			if (value > alpha && nextDepth > depth + 1 && !done) {
				lineDown.clear();
				value = -alphabeta(depth + 1, position, -alpha - 1, -alpha, lineDown, childIrrev, rootIndex);
			}
			if (value > alpha && value < beta && !done) {
				lineDown.clear();
				value = -alphabeta(depth + 1, position, -beta, -alpha, lineDown, childIrrev, rootIndex);
			}
		}
		position.unmake_move(newMove, ms);
		if (repetitionSeen) {
			localRepetitionSeen = true;
			repetitionSeen = false;
		}

		if (done) {
			repetitionSeen = savedRepetitionSeen || localRepetitionSeen;
			return 0;
		}

		if (value >= beta) {
			if (!isCapture) {
				if (!sameMove(killers[ply][0], newMove)) {
					killers[ply][1] = killers[ply][0];
					killers[ply][0] = newMove;
				}
				uint8_t f = static_cast<uint8_t>(newMove.get_from()) & 63;
				uint8_t t = static_cast<uint8_t>(newMove.get_to()) & 63;
				historyTable[f][t] = std::min(6000, historyTable[f][t] + remDepth * remDepth);
			}
			if (!localRepetitionSeen) {
				tte.key = h;
				tte.score = beta;
				tte.depth = static_cast<int16_t>(remDepth);
				tte.flag = TT_BETA;
				tte.bestMove = newMove;
			}
			repetitionSeen = savedRepetitionSeen || localRepetitionSeen;
			return beta;
		}
		if (value > alpha) {
			alpha = value;
			bestLocalMove = newMove;
			hasBestMove = true;
			lineUp = lineDown;
			lineUp.push_front(newMove);
		}
	}

	if (legalMovesCount == 0) {
		repetitionSeen = savedRepetitionSeen || localRepetitionSeen;
		if (inCheck) {
			return -MATE_SCORE + depth;
		}
		return 0;
	}

	if (!done && !localRepetitionSeen) {
		tte.key = h;
		tte.score = alpha;
		tte.depth = static_cast<int16_t>(remDepth);
		tte.flag = (alpha > origAlpha) ? TT_EXACT : TT_ALPHA;
		if (hasBestMove) {
			tte.bestMove = bestLocalMove;
		}
	}

	repetitionSeen = savedRepetitionSeen || localRepetitionSeen;
	return alpha;
}

int Searcher::quiescence_alphabeta(int depth, Position& position, int alpha, int beta) {
	if ((nodes & 255) == 0 && isHardTimeUp()) {
		done = true;
	}
	if (done) {
		return 0;
	}
	if (depth > seldepth) {
		seldepth = depth;
	}

	int v = Eval::Evaluator::getValue(position);
	if (v >= beta) {
		return beta;
	}
	if (v > alpha) {
		alpha = v;
	}
	if (depth - idDepth >= 6) {
		return alpha;
	}

	Move_generator mg;
	Move_container mc = mg.generate_pseudolegal_captures(position);
	int count = static_cast<int>(mc.size());
	if (count == 0) {
		return alpha;
	}

	auto& moves = mc.get_moves_ref();
	int scores[CONTAINER_SIZE];
	for (int i = 0; i < count; ++i) {
		scores[i] = scoreMove(position, moves[i]);
	}

	for (int i = 0; i < count; ++i) {
		int bestIdx = i;
		for (int j = i + 1; j < count; ++j) {
			if (scores[j] > scores[bestIdx]) {
				bestIdx = j;
			}
		}
		if (bestIdx != i) {
			std::swap(moves[i], moves[bestIdx]);
			std::swap(scores[i], scores[bestIdx]);
		}

		const Move& newMove = moves[i];
		Move_state ms;
		position.make_move(newMove, ms);
		if (position.is_in_check(!position.is_white_to_move())) {
			position.unmake_move(newMove, ms);
			continue;
		}
		++nodes;

		const int value = -quiescence_alphabeta(depth + 1, position, -beta, -alpha);
		position.unmake_move(newMove, ms);

		if (done) {
			return 0;
		}

		if (value >= beta) {
			return beta;
		}
		if (value > alpha) {
			alpha = value;
		}
	}
	return alpha;
}
