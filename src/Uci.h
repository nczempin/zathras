#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <algorithm>
#include <cstdlib>

#include "Character.h"
#include "Position.h"
#include "Move_generator.h"
#include "Searcher.h"
#include "Move.h"
#include "Evaluator.h"

#include "Perft_command.h"
#include "Divide_command.h"

extern Position p;
extern Searcher searcher;
extern const std::string VERSION;

namespace Interface {

	using namespace std;

	class Uci {
	public:
		static void stopSearch() {
			searcher.done = true;
			if (searchThread.joinable()) {
				searchThread.join();
			}
		}

		static void waitForSearch() {
			if (searchThread.joinable()) {
				searchThread.join();
			}
		}

		static void parse(string toParse) {
			if (toParse.empty()) {
				return;
			}
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
			else if (startsWith("perft ", toParse) || startsWith("go perft ", toParse)) {
				waitForSearch();
				string pattern = startsWith("go perft ", toParse) ? "go perft " : "perft ";
				size_t index = toParse.find(pattern);
				string depth_param = toParse.substr(index + pattern.length());

				size_t depth = 6;
				if (depth_param.length() > 0) {
					try {
						depth = std::stoi(depth_param);
					} catch (const std::exception&) {
						cout << "Invalid depth parameter. Using depth 6." << endl;
					}
				}
				Perft_command pc{p, depth};
				pc.execute();
			}
			else if (startsWith("divide ", toParse)) {
				waitForSearch();
				string pattern = "divide ";
				size_t index = toParse.find(pattern);
				string depth_param = toParse.substr(index + pattern.length());
				size_t depth = 1;
				if (depth_param.length() > 0) {
					try {
						depth = std::stoi(depth_param);
					} catch (const std::exception&) {
						cout << "Invalid depth parameter. Using depth 1." << endl;
					}
				}
				Divide_command dc{p, depth};
				dc.execute();
			}
			else if (toParse == "isready") {
				cout << "readyok" << endl;
			}
			else if (toParse == "eval") {
				waitForSearch();
				int v = Eval::Evaluator::getValue(p);
				cout << "Value: " << v << endl;
			}
			else if (startsWith("go", toParse)) {
				waitForSearch();
				long long wtime = -1, btime = -1, winc = 0, binc = 0, movetime = -1;
				int movestogo = 0, depth = 0;
				bool infinite = false;
				bool hasWtime = false, hasBtime = false, hasWinc = false, hasBinc = false;

				std::istringstream iss(toParse);
				std::string token;
				iss >> token; // "go"
				while (iss >> token) {
					if (token == "wtime" && (iss >> wtime)) hasWtime = true;
					else if (token == "btime" && (iss >> btime)) hasBtime = true;
					else if (token == "winc" && (iss >> winc)) hasWinc = true;
					else if (token == "binc" && (iss >> binc)) hasBinc = true;
					else if (token == "movestogo") iss >> movestogo;
					else if (token == "movetime") iss >> movetime;
					else if (token == "depth") iss >> depth;
					else if (token == "infinite") infinite = true;
				}

				long long softMs = 0;
				long long hardMs = 0;
				long long myTime = p.white_to_move ? wtime : btime;
				long long myInc = p.white_to_move ? winc : binc;
				bool clockProvided = hasWtime || hasBtime || hasWinc || hasBinc;

				if (infinite) {
					softMs = 0;
					hardMs = 0;
				} else if (movetime > 0) {
					softMs = std::max(1LL, movetime - 15);
					hardMs = std::max(1LL, movetime - 10);
				} else if (clockProvided) {
					if (myTime <= 0) {
						softMs = 1;
						hardMs = 1;
					} else {
						long long overhead = std::min(25LL, std::max(5LL, myTime / 10));
						long long avail = std::max(1LL, myTime - overhead);
						int mtg = (movestogo > 0) ? movestogo : 30;
						long long target = avail / mtg + (myInc * 3) / 4;
						softMs = std::max(1LL, std::min(target, avail / 3));
						hardMs = std::max(1LL, std::min(target * 2, avail / 2));
					}
				} else if (depth <= 0) {
					softMs = 500;
					hardMs = 1000;
				}

				searcher.startTimer(softMs, hardMs, depth);
				Position searchPos = p;
				searchThread = std::thread([searchPos]() mutable {
					Move best = searcher.analyze(searchPos);
					std::cout << "bestmove " << Moves::to_string(best) << std::endl;
				});
			}
			else if (startsWith("stop", toParse)) {
				stopSearch();
			}
			else if (startsWith("quit", toParse)) {
				stopSearch();
				exit(0);
			}
			else if (startsWith("ucinewgame", toParse)) {
				waitForSearch();
				p = Position::create_start_position();
				searcher.clearHistory();
				searcher.pushHistory(p);
			}
			else if (startsWith("position", toParse)) {
				waitForSearch();
				string positionString = extractPosition(toParse);
				if (startsWith("startpos", positionString)) {
					p = Position::create_start_position();
				}
				else if (startsWith("fen", positionString)) {
					string positionFen = extractFen(positionString);
					if (positionFen.empty()) {
						return;
					}
					p = Position::create_position(positionFen);
				} else {
					return;
				}
				searcher.resetGameHistory();
				searcher.pushHistory(p);

				string movesString = extractMoves(toParse);
				vector<string> moves = Position::split(movesString, ' ');
				if (!movesString.empty() && !moves.empty())
				{
					for (const string& move : moves) {
						if (!move.empty()) {
							bool irrev = false;
							if (makeMove(p, move, irrev)) {
								searcher.pushHistory(p, irrev);
							}
						}
					}
				}
			}
			else if (toParse == "sp" || toParse == "d") {
				stopSearch();
				p.print(cout);
			}
			else {
				cout << "Unknown command: '" << toParse << "'" << endl;
				cout << "Available commands: uci, isready, position, go, perft, divide, d (display), sp (show position), quit" << endl;
			}
		}
	private:
		inline static std::thread searchThread;

		static bool startsWith(string smallString, string bigString) {
			return bigString.compare(0, smallString.length(), smallString) == 0;
		}

		static string extractMoves(string parameters)
		{
			string pattern = " moves ";
			size_t index = parameters.find(pattern);
			if (index == string::npos)
			{
				return "";
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
				return "";
			}
			string retVal = parameters.substr(index + pattern.length());
			return retVal;
		}

		static string extractFen(string positionString) {
			static const string command = "fen ";
			if (!startsWith(command, positionString) || positionString.length() <= command.length()) {
				return "";
			}
			string retValue = positionString.substr(command.length());
			size_t movesFoundAt = retValue.find("moves ");
			if (movesFoundAt != string::npos) {
				retValue = retValue.substr(0, movesFoundAt);
			}
			while (!retValue.empty() && (retValue.back() == ' ' || retValue.back() == '\r' || retValue.back() == '\n')) {
				retValue.pop_back();
			}
			return retValue;
		}

		static bool makeMove(Position& pos, const string& moveString, bool& irreversible) {
			if (moveString.length() != 4 && moveString.length() != 5) {
				return false;
			}
			if (moveString[0] < 'a' || moveString[0] > 'h' ||
			    moveString[1] < '1' || moveString[1] > '8' ||
			    moveString[2] < 'a' || moveString[2] > 'h' ||
			    moveString[3] < '1' || moveString[3] > '8') {
				return false;
			}
			Move_generator mg;
			Move_container legal = mg.generate_legal_moves(pos);
			const auto& moves = legal.get_moves_ref();
			string normalized = moveString;
			if (normalized.length() == 5 && normalized[4] >= 'A' && normalized[4] <= 'Z') {
				normalized[4] = static_cast<char>(normalized[4] - 'A' + 'a');
			}
			for (size_t i = 0; i < legal.size(); ++i) {
				const Move& m = moves[i];
				if (Moves::to_string(m) == normalized) {
					piece_t moving = pos.get_piece_on(m.get_from());
					piece_t captured = pos.get_piece_on(m.get_to());
					Move_state ms;
					pos.make_move(m, ms);
					irreversible = (captured != 0) ||
					               (m.get_move_type() == EN_PASSANT) ||
					               m.is_promotion() ||
					               (moving == Piece::WHITE_PAWN || moving == Piece::BLACK_PAWN) ||
					               (pos.castling != ms.prev_castling);
					return true;
				}
			}
			return false;
		}
	};
}