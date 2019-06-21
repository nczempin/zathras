#include <vector>
#include <algorithm>
#include <set>
#include <iostream>
#include <cassert>
#include <cstdlib>

#include "Searcher.h"
#include "Evaluator.h"
#include "Move.h"
#include "Info.h"


Searcher::Searcher(void)
{
}


Searcher::~Searcher(void)
{
}


Move Searcher::findBestmove(array<Move, Move_container::SIZE> moves, Position position) {
	// assumption: moves.size() > 1
	resetClock();
	idDepth = 1;
	int maxIdDepth = 0;
	Info::seldepth = 0;
	Info::nodes = 0;
	Move lastIterationBestMove;
	done = false;
	deque<Move> lineDown;
	list<Move> sortedMoves;
	list<Move> otherMoves;
	int movesSize = 0;
	do {
		lastIterationBestMove = bestMove;
		oldBestValue = bestValue;
		bestValue = -9999999;
		sortedMoves.clear();
		otherMoves.clear();
		Info::currmovenumber = 0;
		for (Move& move : moves) {
			// TODO this is a workaround because of garbage
			if (move.get_moving_piece() == 0) {
				break;
			}
			//Position newPos = position;
			Move_state ms;
			position.make_move(move, ms);
			++Info::nodes;
			Info::currmove = move;
			Info::currmovenumber++;
			printInfo();

			//cout << "trying " << move.toString() << endl;
			int value = -alphabeta(1, position, -9999999, -bestValue, lineDown);
			if (timeUp()) {
				//return lastIterationBestMove;
				done = true;
				break;
			}
			move.value = value;
			if (value > bestValue) {
				//cout << "inserting " << move.toString() << "@" << value << endl;
				sortedMoves.push_front(move);
				bestValue = value;
				bestMove = move;
				pv = lineDown;
				pv.push_front(move);
				printInfo();
				if (bestValue > 80000) {
					done = true;
					break;
				}
			}
			else {
				otherMoves.push_front(move);
			}
			position.unmake_move(move, ms);
		}

		printInfo();
		if (maxIdDepth > 0 && idDepth > maxIdDepth) {
			done = true;
		}
		++idDepth;
		//		moves.clear();
				//static Move emptyMove;
				//moves.fill(emptyMove);
		movesSize = 0;
		//cout << "sortedMoves.size(): " << sortedMoves.size() << endl;
		for (Move move : sortedMoves) {
			if (movesSize < Move_container::SIZE) {
				moves[movesSize++] = move;
			}
			else throw movesSize;
			//			moves.push_back(move);
						//cout << "pushing back " << move.toString() <<" @ " << move.value <<  endl;

		}
		for (Move move : otherMoves) {
			if (movesSize < Move_container::SIZE) {
				moves[movesSize++] = move;
			}
			else throw movesSize;
			//moves.push_back(move);
		}
	} while (!done);
	return bestMove;
}
int Searcher::alphabeta(int depth, Position& position, int alpha, int beta, deque<Move>& lineUp) {
	if (done || (timeUp())) {
		//	timeIsUp = true;
		done = true;
		int value = Evaluator::getValue(position);
		return value;
	}
	int value = 0;
	if (depth >= idDepth) {
		deque<Move> lineDown;
		value = quiescence_alphabeta(depth, position, alpha, beta, lineDown);
		lineUp = lineDown;
		return value;
	}
	Move_generator mg;
	Move_container mc = mg.generate_legal_moves(position, 1);
	auto moves = mc.get_moves();// AllMoves(position, moves);
	int actualMoves = 0;
	int moveCount = 0;
	for (const Move& newMove : moves) {
		if (moveCount++ >= mc.size()) {
			break;
		}

		//expensive way to make next move
		//Position nextPos = position;// .copyPosition();
		Move_state ms;
		position.make_move(newMove, ms);
		++Info::nodes;
		//cout << "making " << newMove.toString() << endl;
		deque<Move> lineDown;
		value = -alphabeta(depth + 1, position, -beta, -alpha, lineDown);
		position.unmake_move(newMove, ms);



		++actualMoves;

		if (value >= beta) {
			//cout << "beta cutoff " << value<< " >= "<< beta <<": "<<newMove.toString()<<endl;
			return beta;
		}
		if (value > alpha) {
			//cout << "new best: " << newMove.toString() << ", " << value << " > " << alpha << endl;
			alpha = value;
			lineUp = lineDown;
			lineUp.push_front(newMove);


			//bestMove = newMove;
			if (value > 800000) {
				return value;
			}
		}
		//}

		if (done || timeUp()) {
			done = true;
			return alpha;
		}

	}
	//if (actualMoves == 0) {
	//	//auto moves = mg.generate_legal_moves(position, 1).get_moves();// generateLegalMoves(position);
	//	//if (moves.size() == 0) {
	//		//cout << "no more moves!" << endl;
	//		//position.print();
	//	if (position.is_in_check(!position.is_white_to_move())) {
	//		return -888888; // Checkmate
	//	}
	//	else {
	//		return 0; // Stalemate
	//	}
	//	//}
	//}
	return alpha;
}

static bool shouldBeIgnored(Position nextPos, Move newMove, int capture, int capturing) {
	return false;//TODO (abs(capturing) > abs(capture)) && ((capturing != 3) || (capture != 2)); //TODO: && (!nextPos.enPrise(newMove));
}

int Searcher::quiescence_alphabeta(int depth, Position& position, int alpha, int beta, deque<Move>& lineUp) {
	//ValidFlag bestMoveValidFlag = new ValidFlag();
	int v = Evaluator::getValue(position);

	if (depth > Info::seldepth) {
		Info::seldepth = depth;
		printInfo();
	}

	if (v >= beta)
		return beta;
	if (v > alpha) {
		//bestMoveValidFlag.setNr(1);
		alpha = v;
	}
	//kingCapture = false;
	int loopCount = 0;

	Move_generator mg;

	auto mc = mg.generate_legal_captures(position, 1);
	auto moves = mc.get_moves();

	//vector<Move> legalMoves = MoveGenerator::removeIllegalMoves(moves);
	for (const Move& newMove : moves) {
		if (newMove.get_moving_piece() == 0) {
			break;
		}
		int capture = newMove.get_captured();
		assert(capture != 0);
		int capturing = newMove.get_moving_piece();// abs(position.board[newMove.get_from()]);

		if (!shouldBeIgnored(position, newMove, capture, capturing)) {
			
			Move_state ms;
			position.make_move(newMove, ms);
			++Info::nodes;
			deque<Move> lineDown;
			int value = -quiescence_alphabeta(depth + 1, position, -beta, -alpha, lineDown);
			position.unmake_move(newMove, ms);

			
			if (value >= beta) {
				return beta;
			}
			if (value > alpha) {
				alpha = value;
				lineUp = lineDown;
				lineUp.push_front(newMove);

			}

		}

		return alpha;
	}
}