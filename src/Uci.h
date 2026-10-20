#pragma once

#include <iostream>
#include <chrono>
#include <future>
#include <thread>
#include <sstream>
#include <algorithm>

#include "Character.h"
#include "Position.h"
#include "Move_generator.h"
#include "Searcher.h"
#include "Move.h"
#include "Evaluator.h"

#include "Info.h"
#include "Perft_command.h"
#include "Divide_command.h"

extern Position p;
extern future<Move> fut;
extern Searcher searcher;
extern const std::string VERSION;

namespace Interface {

	using namespace std;

	class Uci {
	public:

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
				int v = Eval::Evaluator::getValue(p);
				cout << "Value: " << v << endl;
			}
			else if (startsWith("go", toParse)) {
				long long wtime = -1, btime = -1, winc = 0, binc = 0, movetime = -1;
				int movestogo = 0, depth = 0;

				std::istringstream iss(toParse);
				std::string token;
				iss >> token; // "go"
				while (iss >> token) {
					if (token == "wtime") iss >> wtime;
					else if (token == "btime") iss >> btime;
					else if (token == "winc") iss >> winc;
					else if (token == "binc") iss >> binc;
					else if (token == "movestogo") iss >> movestogo;
					else if (token == "movetime") iss >> movetime;
					else if (token == "depth") iss >> depth;
				}

				long long softMs = 0;
				long long hardMs = 0;
				long long myTime = p.white_to_move ? wtime : btime;
				long long myInc = p.white_to_move ? winc : binc;

				if (movetime > 0) {
					softMs = std::max(1LL, movetime - 15);
					hardMs = std::max(1LL, movetime - 10);
				} else if (myTime > 0) {
					long long overhead = std::min(25LL, std::max(5LL, myTime / 10));
					long long avail = std::max(1LL, myTime - overhead);
					int mtg = (movestogo > 0) ? movestogo : 30;
					long long target = avail / mtg + (myInc * 3) / 4;
					softMs = std::max(1LL, std::min(target, avail / 3));
					hardMs = std::max(1LL, std::min(target * 2, avail / 2));
				} else if (depth <= 0) {
					softMs = 500;
					hardMs = 1000;
				}

				Info::timePerMove = static_cast<int>(hardMs);
				searcher.startTimer(softMs, hardMs, depth);
				Move best = searcher.analyze(p);
				cout << "bestmove " << Moves::to_string(best) << endl;
			}
			else if (startsWith("stop", toParse)) {
				searcher.done = true;
			}
			else if (startsWith("quit", toParse)) {
				searcher.done = true;
				exit(0);
			}
			else if (startsWith("ucinewgame", toParse)) {
				p = Position::create_start_position();
				searcher.clearHistory();
				searcher.pushHistory(p);
			}
			else if (startsWith("position", toParse)) {
				string positionString = extractPosition(toParse);
				if (startsWith("startpos", positionString)) {
					p = Position::create_start_position();
				}
				else if (startsWith("fen", positionString)) {
					string positionFen = extractFen(positionString);
					p = Position::create_position(positionFen);
				}
				searcher.clearHistory();
				searcher.pushHistory(p);

				string movesString = extractMoves(toParse);
				vector<string> moves = Position::split(movesString, ' ');
				if (!movesString.empty() && !moves.empty())
				{
					for (const string& move : moves) {
						if (!move.empty()) {
							makeMove(p, move);
							searcher.pushHistory(p);
						}
					}
				}
			}
			else if (toParse == "sp" || toParse == "d") {
				p.print(cout);
			}
			else {
				cout << "Unknown command: '" << toParse << "'" << endl;
				cout << "Available commands: uci, isready, position, go, perft, divide, d (display), sp (show position), quit" << endl;
			}
		}
	private:
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
			static string command = "fen ";
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

		static void makeMove(Position& pos, string moveString) {
			if (moveString.length() < 4) {
				return;
			}
			Move m = convert_move(moveString, pos);
			Move_state ms;
			pos.make_move(m, ms);
		}

		static Move convert_move(string move, const Position& pos) {
			square_t from = static_cast<square_t>(Util::decodeSquare(move.substr(0, 2)));
			square_t to = static_cast<square_t>(Util::decodeSquare(move.substr(2, 2)));
			string promotedTo = (move.length() > 4) ? move.substr(4) : "";

			move_type_t mt = NONE;
			piece_t moving = pos.get_piece_on(from);
			piece_t captured = pos.get_piece_on(to);

			if (!promotedTo.empty()) {
				char promo = promotedTo[0];
				switch (promo) {
					case 'q':
					case 'Q':
						mt = PROMOTION_QUEEN;
						break;
					case 'r':
					case 'R':
						mt = PROMOTION_ROOK;
						break;
					case 'b':
					case 'B':
						mt = PROMOTION_BISHOP;
						break;
					case 'n':
					case 'N':
						mt = PROMOTION_KNIGHT;
						break;
					default:
						break;
				}
			}

			if (moving == Piece::WHITE_PAWN) {
				if (captured == 0) {
					if (to - from == 9 || to - from == 7) {
						mt = EN_PASSANT;
					}
				}
			}
			else if (moving == Piece::BLACK_PAWN) {
				if (captured == 0) {
					if (from - to == 9 || from - to == 7) {
						mt = EN_PASSANT;
					}
				}
			}
			return Move(from, to, mt);
		}
	};
}