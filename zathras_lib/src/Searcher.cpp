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
//using namespace zathras::interface; //TODO

Searcher::Searcher(void) : bestMove(0)
{
}


Searcher::~Searcher(void)
{
}


Move Searcher::findBestmove(move_container_t moves, Position position) {
	// assumption: moves.size() > 1
	
	idDepth = 1;
	int maxIdDepth = 0;
	//TODOInfo::seldepth = 0;
	//TODOInfo::nodes = 0;
	done = false;
	deque<Move> lineDown;
	do {
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
			//if (Util::time_up()) {
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
		
	} while (!done);
	return bestMove;
}

int Searcher::alphabeta(int depth, Position& position, int alpha, int beta, deque<Move>& lineUp) {
	if (done /* ||(Util::time_up())*/) {
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
	size_t moveCount = 0;
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

		if (done ) /*||Util::time_up()*/ {
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
		//const int& capturing = newMove.get_moving_piece();// abs(position.board[newget_from(move)]);

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
