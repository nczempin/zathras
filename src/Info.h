#pragma once
#include <deque>
#include <chrono>

#include "../zathras_lib/src/Move.h"
#include <iostream>

namespace Interface {
	using namespace std;
	using namespace Moves;
	class Info {
	public:
		static int timePerMove; //TODO this is only a temporary home


		static int seldepth;
		static Move currmove;
		static int currmovenumber;
		static int nodes;
		static int nps;
		static chrono::system_clock::time_point start;
		static void updateNps() {
			chrono::duration<double> elapsed_seconds = chrono::system_clock::now() - Info::start;
			double seconds = (elapsed_seconds.count());
			Info::nps = static_cast<int>(Info::nodes / seconds);
		}
		static void printInfo(int bestValue, int oldBestValue, int idDepth, const deque<Move>& pv) {
			cout << "info depth " << idDepth;
			cout << " seldepth " << Info::seldepth;
			cout << " currmove " << Moves::to_string(Info::currmove);
			cout << " currmovenumber " << Info::currmovenumber;
			cout << " nodes " << Info::nodes;
			updateNps();
			cout << " nps " << Info::nps;
			cout << " score ";
			if (bestValue > 80000) {
				cout << "mate " << idDepth / 2;
			}
			else {
				cout << "cp " << oldBestValue;
			}
			cout << " pv ";
			for (const Move& m : pv) {
				cout << Moves::to_string(m) << " ";
			}
			cout << endl;
		}
	};
}