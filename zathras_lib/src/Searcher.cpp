#include <vector>
#include <algorithm>
#include <set>
#include <iostream>
#include <cassert>
#include <cstdlib>

#include "Searcher.h"
#include "Evaluator.h"
#include "Move.h"


#include "misc.h"
//using namespace Interface; //TODO

Searcher::Searcher(void)
{
}


Searcher::~Searcher(void)
{
}


Move Searcher::findBestmove(array<Move, Move_container::SIZE> moves, Position position) {
	// assumption: moves.size() > 1
	
	idDepth = 1;
	int maxIdDepth = 0;
	//TODOInfo::seldepth = 0;
	//TODOInfo::nodes = 0;
	Move lastIterationBestMove;
	done = false;
	deque<Move> lineDown;
	
	int movesSize = 0;
	do {
		lastIterationBestMove = bestMove;
		oldBestValue = bestValue;
		bestValue = -9999999;
		

		//TODOInfo::currmovenumber = 0;
		for (Move& move : moves) {
			// TODO this is a workaround because of garbage
			/*if (move.get_moving_piece() == 0) {
				break;
			}*/
			//Position newPos = position;
			Move_state ms;
			position.make_move(move, ms);



			//++Info::nodes;
			//Info::currmove = move;
			//Info::currmovenumber++;
			//
			//
			////printInfo();

			//cout << "trying " << move.toString() << endl;
			int value = -alphabeta(1, position, -9999999, -bestValue, lineDown);
			//TODO
			//if (Util::timeUp()) {
			//	//return lastIterationBestMove;
			//	done = true;
			//	break;
			//}
			//TODO move with value move.value = value;
			if (value > bestValue) {
				//cout << "inserting " << move.toString() << "@" << value << endl;
				

				bestValue = value;
				bestMove = move;
				pv = lineDown;
				pv.push_front(move);
				
				//printInfo();
				
				if (bestValue > 80000) {
					done = true;
					break;
				}
			}
			

			position.unmake_move(move, ms);
		}

		//printInfo();
		
		
		if (maxIdDepth > 0 && idDepth > maxIdDepth) {
			done = true;
		}
		++idDepth;
		//		moves.clear();
				//static Move emptyMove;
				//moves.fill(emptyMove);
		movesSize = 0;
		
	} while (!done);
	return bestMove;
}
int Searcher::alphabeta(int depth, Position& position, int alpha, int beta, deque<Move>& lineUp) {
	if (done /* ||(Util::timeUp())*/) {
		//	timeIsUp = true;
		done = true;
		const int& value = Eval::Evaluator::getValue(position);
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
		//TODO++Info::nodes;
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

		if (done ) /*||Util::timeUp()*/ {
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
	int v = Eval::Evaluator::getValue(position);

	//if (depth > Info::seldepth) {
	//	Info::seldepth = depth;
	//	
	//	//printInfo();
	//}

	if (v >= beta)
		return beta;
	if (v > alpha) {
		//bestMoveValidFlag.setNr(1);
		alpha = v;
	}
	//kingCapture = false;
	
	Move_generator mg;

	auto mc = mg.generate_legal_captures(position, 1);
	auto moves = mc.get_moves();

	//vector<Move> legalMoves = MoveGenerator::removeIllegalMoves(moves);
	for (const Move& newMove : moves) {
		//if (newMove.get_moving_piece() == 0) {
		//	break;
		//}
		//const int& capture = newMove.get_captured();
		//assert(capture != 0);
		//const int& capturing = newMove.get_moving_piece();// abs(position.board[newMove.get_from()]);

		//if (!shouldBeIgnored(position, newMove, capture, capturing)) {

			Move_state ms;
			position.make_move(newMove, ms);
			//TODO++Info::nodes;
			deque<Move> lineDown;
			const int value = -quiescence_alphabeta(depth + 1, position, -beta, -alpha, lineDown);
			position.unmake_move(newMove, ms);


			if (value >= beta) {
				return beta;
			}
			if (value > alpha) {
				alpha = value;
				lineUp = lineDown;
				lineUp.push_front(newMove);

			}

	//	}
	}
	return alpha;
}
