#pragma once

#include <iostream>
#include <chrono>
#include <future>
#include <thread>


#include "Character.h"
#include "Position.h"
#include "Move_generator.h"
#include "Searcher.h"
#include "Move.h"
#include "Evaluator.h"

#include "info.h"
#include "Perft_command.h"

extern Position p;


extern future<Move> fut;
extern Searcher searcher;


namespace Interface {

	using namespace std;

	class Uci {
	public:

		static void parse(string toParse);
	private:
		static bool startsWith(string smallString, string bigString) {
			return bigString.compare(0, smallString.length(), smallString) == 0;
		}


		static void stopBrain();
		
		static void finishBrain();
			
		static void startBrain();
		
		static string extractMoves(string parameters);
		static string extractPosition(string parameters);
		static string extractFen(string positionString);
		static void resetClock();
		static Move asyncAnalyze();
		static void make_uci_move(Position& pos, string textual_move);
	    static Move convert_move(string moveString, piece_t * board);		
	};
}