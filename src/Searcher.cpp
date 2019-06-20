#include <vector>
#include <algorithm>
#include <set>
#include <iostream>
#include <cassert>

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


Move Searcher::findBestmove(array<Move,Move_container::SIZE> moves, Position position) {
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
		for (Move move : moves) {
			// TODO this is a workaround because of garbage
			if (move.get_moving_piece() == 0) {
				break; 
			}
			Position newPos = position;
			Move_state ms;
			newPos.make_move(move,ms);
			++Info::nodes;
			Info::currmove = move;
			Info::currmovenumber++;
			updateNps();
			cout << "info ";
			cout << " currmove " << Info::currmove.to_string();
			cout << " currmovenumber " << Info::currmovenumber;
			cout << " nodes " << Info::nodes;
			cout << " nps " << Info::nps << endl;
			
			//cout << "trying " << move.toString() << endl;
			int value = -alphabeta(1, newPos, -9999999, -bestValue, lineDown);
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
				//}else if (value==bestValue){
				//	//cout << "inserting alt." << move.toString() << "@" << value << endl;
				//	sortedMoves.push_front(move);
			}
			else {
				otherMoves.push_front(move);
			}

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
		//cout << "sortedMoves: ";
		//for (Move move : sortedMoves){
		//	cout << move.toString()<<" ";
		//}
		//cout << endl;
		//cout << "otherMoves: ";
		//for (Move move : otherMoves){
		//	cout << move.toString()<<" ";
		//}
		//cout << endl;
		//cout << "moves: ";
		//for (Move move : moves){
		//	cout << move.toString()<<" ";
		//}
		//cout << endl;
	} while (!done);
	return bestMove;
}
int Searcher::alphabeta(int depth, Position position, int alpha, int beta, deque<Move>& lineUp) {
	if (done || (timeUp())) {
		//	timeIsUp = true;
		return 0;
	}
	int value = 0;
	if (depth >= idDepth) {
		//deque<Move> lineDown;
		//value = quiescence_alphabeta(depth, position, alpha, beta, lineDown);
		//lineUp = lineDown;
		value = Evaluator::getValue(position);
		return value;
	}
	//vector<Move> moves;
	//moves.reserve(40);
	Move_generator mg;
	Move_container mc = mg.generate_legal_moves(position, 1);
	auto moves = mc.get_moves();// AllMoves(position, moves);
	//vector<Move> moves = MoveGenerator::generateLegalMoves(position);
	int actualMoves = 0;
	int moveCount = 0;
	for (Move newMove : moves) {
		if (moveCount++ >= mc.size()) {
			break;
		}
		int capture = newMove.get_captured();// captured;
		//assert(capture >= 0);
		if (capture == 6) {
			//cout << "king captured: " << newMove.toString() << endl;
			kingCapture = true;
			//	//	illegalCount += 1;
			return 666663;
		}
		//expensive way to make next move
		Position nextPos = position;// .copyPosition();
		Move_state ms;
		nextPos.make_move(newMove, ms);
		++Info::nodes;
		//cout << "making " << newMove.toString() << endl;
		deque<Move> lineDown;
		value = -alphabeta(depth + 1, nextPos, -beta, -alpha, lineDown);
		// back to "position" = expensive take back move
		if (kingCapture) {
			//cout << "caught king capture: " << newMove.toString() << endl;
			kingCapture = false;
			//ignore this move, though
		}
		else {
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
		}

		if (timeUp()) {
			done = true;
			return alpha;
		}

	}
	if (actualMoves == 0) {
		//auto moves = mg.generate_legal_moves(position, 1).get_moves();// generateLegalMoves(position);
		//if (moves.size() == 0) {
			//cout << "no more moves!" << endl;
			//position.print();
			if (position.is_in_check(!position.is_white_to_move())) {
				return -888888; // Checkmate
			}
			else {
				return 0; // Stalemate
			}
		//}
	}
	return alpha;
}
//
//static bool shouldBeIgnored(Position nextPos, Move newMove, int capture, int capturing) {
//	return (capturing > capture) && ((capturing != 3) || (capture != 2)) && (!nextPos.enPrise(newMove));
//}
//
//int Searcher::quiescence_alphabeta(int depth, Position position, int alpha, int beta, deque<Move>& lineUp) {
//	//ValidFlag bestMoveValidFlag = new ValidFlag();
//	if (done || (timeUp())) {
//		//	timeIsUp = true;
//		return 0;
//	}
//	if (depth > Info::seldepth) {
//		Info::seldepth = depth;
//		printInfo();
//	}
//	//if (nextPos == null)
//	//	try {
//	//		Info.qs_nodes += 1L;
//	//		nextPos = Position.createPosition(position, move);
//	//	} catch (ThreeRepetitionsAB e) {
//	//		System.err.println("three repetitions. what to do?");
//	//		return 0;
//	//	}
//	int v = Evaluator::getValue(position);
//	//return v;
//	/*if (depth>20){
//	 return -v;
//	 }*/
//	if (v >= beta)
//		return beta;
//	if (v > alpha) {
//		//bestMoveValidFlag.setNr(1);
//		alpha = v;
//	}
//	//kingCapture = false;
//	int loopCount = 0;
//	vector<Move> moves;
//	moves.reserve(10);
//	Move_generator::generateAllCaptures(position, moves);
//
//	//vector<Move> legalMoves = MoveGenerator::removeIllegalMoves(moves);
//	for (Move newMove : moves) {
//		int capture = newMove.captured;
//		assert(capture > 0);
//		int capturing = abs(position.board[newMove.from]);
//		if (capture == 6) {
//			kingCapture = true;
//			//	//	illegalCount += 1;
//			//cout << "captured King: " << newMove.toString() << endl;
//			return -666663;
//		}
//		if (!shouldBeIgnored(position, newMove, capture, capturing)) {
//			//moveStack.push(newMove);
//			//vector<Move> downPv;
//			Position nextPos = position.copyPosition();
//			nextPos.makeMove(newMove);
//			++Info::nodes;
//			deque<Move> lineDown;
//			int value = -quiescence_alphabeta(depth + 1, nextPos, -beta, -alpha, lineDown);
//			if (kingCapture) {
//				//	illegalCount += 1;
//				//	moveStack.pop();
//				kingCapture = false;
//				//cout << "ignoring: " << newMove.toString() << endl;
//			}
//			else {
//				loopCount++;
//				if (value >= beta) {
//					//moveStack.pop();
//					return beta;
//				}
//				//bestMoveValidFlag.setNr(-1);
//				if (value > alpha) {
//					alpha = value;
//					lineUp = lineDown;
//					lineUp.push_front(newMove);
//					/*		cout << "new q best: " << newMove.toString() << endl;
//					 for(Move move: lineUp){
//					 cout << move.toString() << " ";
//					 }
//					 cout << endl;*/
//					 //upPv.clear();
//					 //upPv.add(newMove);
//					 //upPv.addAll(downPv);
//				}
//				//moveStack.pop();
//			}
//			//}
//		}
//		//if (loopCount==0){
//		//	vector<Move> legalMoves = MoveGenerator::generateLegalMoves(position);
//		//	if (legalMoves.size()==0){
//		//		//cout << "no more moves!" << endl;
//		//		//position.print();
//		//		if (position.isReceivingCheck()){
//		//			return -888888; // Checkmate
//		//		}else{
//		//			return 0; // Stalemate
//		//		}
//		//	}
//	}
//	/*	if (loopCount == 0) {
//	 nextPos.initNonCaptureMoveGenerator();
//	 boolean legal = false;
//	 while (nextPos.hasNextNonCaptureMove()) {
//	 Move testMove = nextPos.nextNonCaptureMove();
//	 Info.quiescentMateCheckNodes += 1;
//	 Position tp = Position.createTestPosition(nextPos, testMove);
//	 if (!tp.isGivingCheck()) {
//	 legal = true;
//	 break;
//	 }
//	 }
//	 if (!legal) {
//	 SortedSet lMoves = nextPos.generateLegalMoves();
//	 if (lMoves.size() == 0) {
//	 if (nextPos.isMate()) {
//	 int value = -90000 + depth;
//	 return -value;
//	 }
//	 return 0;
//	 }
//	 }
//	 alpha = v;
//	 }*/
//	return alpha;
//}
