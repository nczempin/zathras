#pragma once

#include "global.h"
//  main.cpp


#include <iostream>
#include <chrono>
#include <future>
#include <thread>

#include "global.h"
#include "Character.h"
#include "Position.h"
#include "Move_generator.h"
#include "Searcher.h"
#include "Move.h"
#include "Perft_command.h"
#include "Evaluator.h"

using namespace std;

extern Position p;

extern long timePerMove;
extern future<Move> fut;
extern Searcher searcher;

class Uci {
public:

	static void parse(string toParse) {
		// for debugging
		if (toParse == ".") {
			toParse = "position fen r3r1k1/1p3nqp/2pp4/p4p2/Pn3P1Q/2N4P/1PPR2P1/3R1BK1 w - - 0 1";
		}
		if (toParse == "ps") {
			toParse = "position startpos";
		}
		if (toParse == "uci") {
			std::cout << "id name Zathras " + VERSION << endl;
			cout << "id author Nicolai Czempin" << endl;
			cout << "uciok" << endl;
		}
		else if (startsWith("perft ", toParse)) { //TODO not really UCI
			Perft_command pc;
			pc.execute();
			//char perftDepthParameter = toParse[6];//'4'; //TODO extract from toParse
			//int perftDepth = Character::getNumericValue(perftDepthParameter);

			//chrono::time_point<chrono::system_clock> end;


			////TODO do this more elegantly
			//for (int i = 0; i < perftDepth; ++i) {
			//	Info::start = chrono::system_clock::now();
			//	int nodes = perft(p, i + 1);
			//	end = chrono::system_clock::now();
			//	chrono::duration<double> elapsed_seconds = end - Info::start;
			//	cout << i + 1 << ", " << nodes << " @ " << elapsed_seconds.count() << endl;
			//	//time_t end_time = chrono::system_clock::to_time_t(end);
			//}
			//cout << "Done." << endl;
		
		}
		//else if (startsWith("divide ", toParse)) {
		//	char perftDepthParameter = toParse[7];
		//	int perftDepth = Character::getNumericValue(perftDepthParameter);
		//	//TODO do this more elegantly
		//	MoveGenerator mg;
		//	vector<Move> moves = mg.generateLegalMoves(p);
		//	int count = 0;
		//	Position tmpPos = p;
		//	for (Move move : moves) {
		//		tmpPos = p; //simple but inefficient way to undo the move
		//		tmpPos.makeMove(move);

		//		count = perft(tmpPos, perftDepth - 1);
		//		cout << move.toString() << ": " << count << endl;
		//	}

		//	cout << "Done." << endl;

		//}
		else if (toParse == "isready") {
			cout << "readyok" << endl;
		}
		else if (toParse == "eval") {
			int v = Evaluator::getValue(p);
			cout << "Value: " << v << endl;
		}
		else if (startsWith("go", toParse)) {
			/*int wtime = extractIntValue(toParse, "wtime");
			int btime = extractIntValue(toParse, "btime");
			int winc = extractIntValue(toParse, "winc");
			int binc = extractIntValue(toParse, "binc");
			int mtg = extractIntValue(toParse, "movestogo");
			if (mtg == 0) {
				mtg = 25;
			}

			long tpm;
			if (p.white_to_move) {
				tpm = calculateTimePerMove(wtime, winc, mtg);
			}
			else {
				tpm = calculateTimePerMove(btime, binc, mtg);
			}*/

			auto tpm = 5000;
			timePerMove = tpm;

			cout << "tpm: " << tpm << endl;


			/*    this.engine.setMovesToGo(mtg);
			this.engine.setTimes(wtime, btime, winc, binc);
			int depth = extractIntValue(parameters, "depth");
			this.engine.setDepth(depth);*/
			startBrain();
		}
		else if (startsWith("stop", toParse)) {
			stopBrain();
		}
		else if (startsWith("quit", toParse)) {
			exit(0); //TODO more elegance
		}
		else if (startsWith("ucinewgame", toParse)) {
			p.create_start_position(); //TODO warning C26444 avoid unnamed objects with custom construction
			cout << "!" << endl;
		}
		else if (startsWith("position", toParse)) {
			//p.clear();//TODO hopefully the other methods will have taken care of this
			string positionString = extractPosition(toParse);
			if (startsWith("startpos", positionString)) {
				p = p.create_start_position();
			}
			else if (startsWith("fen", positionString)) {
				string positionFen = extractFen(positionString);
				p.create_position(positionFen);//TODO warning C26444 avoid unnamed objects with custom construction
				p.print(cout);
			}
			string movesString = extractMoves(toParse);
			vector<string> moves = split(movesString, ' ');
			//p.clearThreeDraws();
			if ((movesString != "") && (moves.size() != 0))
			{
				for (string move : moves) {
					//p.print();
					

					//cout << "making move: "<<move<<endl;
					p.makeMove(move);
				}
			}
		}
		else if (toParse == "sp") {
			p.print(cout);
		}
		/*else if (toParse == "sm") {
			cout << "Moves: " << endl;
			MoveGenerator mg;
			vector<Move> moves = mg.generateLegalMoves(p);
			for (Move move : moves) {
				move.print(cout);
			}
		}*/
		else {
			cout << "???" << endl;
		}
	}
private:
	static bool startsWith(string smallString, string bigString) {
		return bigString.compare(0, smallString.length(), smallString) == 0;
	}


	static void stopBrain()
	{
		searcher.done = true; // ask the thread to finish
		fut.get(); //wait until the thread is finished

		//if ((brainThread != null) && (brainThread.isAlive())) {
		//	String move = this.brain.getBestMoveSoFar();
		//	String toPrint; if (move == null) {
		//		toPrint = printMove(null);
		//	} else
		//		toPrint = printMove(move);
		//	System.out.println(toPrint);
		//	brainThread.stop();
		//}
		//if ((printInfoThread != null) && (printInfoThread.isAlive()))
		//	printInfoThread.stop();
	}

	static void startBrain() {
		//stopBrain(false);

		fut = async(asyncAnalyze);
		//async(printInfo);
		//	this_thread::sleep_for (chrono::seconds(10));
		//Move bestMoveSoFar = s.bestMove;
		//cout << "best move so far " << bestMoveSoFar.toString() << endl;
		//this_thread::sleep_for (chrono::seconds(10));
		//Move move = fut.get();
		//cout << "info string after fut.get" << endl;
		//cout << "bestmove " << move.toString() << endl;

		/*printInfoThread = new Thread()
		{
		public void run() {
		while (AbstractEngine.brainThread.isAlive()) {
		Options.protocol.printInfo();
		try {
		Thread.sleep(1000L);
		} catch (InterruptedException e) {
		e.printStackTrace();
		}

		}

		}


		};*/
		//printInfoThread.start();
	}

	static string extractMoves(string parameters)
	{
		string pattern = " moves ";
		size_t index = parameters.find(pattern);
		if (index == string::npos)
		{
			return ""; //not found
		}
		string moves = parameters.substr(index + pattern.length());
		return moves;
	}
	static string extractPosition(string parameters)
	{
		string pattern = "position ";
		size_t index = parameters.find(pattern);
		if (index == string::npos)
		{
			return ""; //not found
		}
		string retVal = parameters.substr(index + pattern.length());
		return retVal;
	}


	static string extractFen(string positionString) {
		static string command = "fen ";
		string retValue = positionString.substr(command.length()); // remove "fen " TODO: more fail-safe / defensive
		//remove moves
		size_t movesFoundAt = retValue.find("moves ");
		if (movesFoundAt != string::npos) {
			retValue = retValue.substr(0, movesFoundAt);
		}
		return retValue;
	}


	static Move asyncAnalyze() {
		Move bestmove = searcher.analyze(p);
		searcher.printInfo();
		//cout << "info string after fut.get" << endl;
		cout << "bestmove " << bestmove.to_string() << endl;
		return bestmove;
	}
};