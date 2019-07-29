#pragma once


//  main.cpp


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
extern const std::string VERSION;

namespace Interface {

	using namespace std;

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
				string pattern = "perft ";
				size_t index = toParse.find(pattern);

				string depth_param = toParse.substr(index + pattern.length());

				uint8_t depth = depth_param.length() > 0 ? std::stoi(depth_param) : 6;
				Perft_command pc{p,  depth };
				pc.execute();
				//char perftDepthParameter = toParse[6];//'4'; //TODO extract from toParse
				//int perftDepth = Character::getNumericValue(perftDepthParameter);

				//chrono::time_point<chrono::system_clock> end;


				////TODO do this more elegantly
				//for (int i = 0; i < perftDepth; ++i) {
				//	Info::start = chrono::system_clock::now();
				//	int nodes = perft(pos, i + 1);
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
			//	vector<Move> moves = mg.generateLegalMoves(pos);
			//	int count = 0;
			//	Position tmpPos = pos;
			//	for (Move textual_move : moves) {
			//		tmpPos = pos; //simple but inefficient way to undo the textual_move
			//		tmpPos.make_uci_move(textual_move);

			//		count = perft(tmpPos, perftDepth - 1);
			//		cout << textual_move.toString() << ": " << count << endl;
			//	}

			//	cout << "Done." << endl;

			//}
			else if (toParse == "isready") {
				cout << "readyok" << endl;
			}
			else if (toParse == "eval") {
				int v = Eval::Evaluator::getValue(p);
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
				if (pos.white_to_move) {
					tpm = calculateTimePerMove(wtime, winc, mtg);
				}
				else {
					tpm = calculateTimePerMove(btime, binc, mtg);
				}*/

				auto tpm = 5000;

				Info::timePerMove = tpm;

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
				write_start_position(p); //TODO warning C26444 avoid unnamed objects with custom construction
				//cout << "!" << endl;
			}
			else if (startsWith("position", toParse)) {
				//pos.clear();//TODO hopefully the other methods will have taken care of this
				string positionString = extractPosition(toParse);
				if (startsWith("startpos", positionString)) {
					write_start_position(p);
				}
				else if (startsWith("fen", positionString)) {
					string positionFen = extractFen(positionString);
					write_position(p,positionFen);//TODO warning C26444 avoid unnamed objects with custom construction
					p.print(cout);
				}
				string movesString = extractMoves(toParse);
				vector<string> moves = Util::split(movesString, ' ');
				//pos.clearThreeDraws();
				if ((movesString != "") && (moves.size() != 0))
				{
					for (string move : moves) {
						//pos.print();


						//cout << "making textual_move: "<<textual_move<<endl;
						make_uci_move(p, move);
					}
				}
			}
			else if (toParse == "sp") {
				p.print(cout);
			}
			/*else if (toParse == "sm") {
				cout << "Moves: " << endl;
				MoveGenerator mg;
				vector<Move> moves = mg.generateLegalMoves(pos);
				for (Move textual_move : moves) {
					textual_move.print(cout);
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
			if (!searcher.done) {
				searcher.done = true; // ask the thread to finish
				Move move = fut.get(); //wait until the thread is finished
				cout << "hurz!" << endl;
				cout << "bestmove " << to_string(move) << endl;

			}
			else {
				cout << "engine already stopped" << endl;
			}
		}
		static void finishBrain() {
			cout << "schnurzel" << endl;
			this_thread::sleep_for(chrono::milliseconds(Info::timePerMove));
			if (!searcher.done) {
				searcher.done = true; // ask the searcher to finish
				cout << "purzel" << endl;
				Move move = fut.get(); //wait until the thread is finished
				cout << "hurzel" << endl;
				cout << "bestmove " << to_string(move) << endl;
			}
		}
		static void startBrain() {

			searcher.done = false;
			fut = async(asyncAnalyze);
			thread(finishBrain).detach();


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

		void static resetClock() {
			Interface::Info::start = chrono::system_clock::now();
		}
		static Move asyncAnalyze() {
			resetClock();


			Move bestmove = searcher.analyze(p);

			return bestmove;
		}


		void static make_uci_move(Position& pos, string textual_move) {
			piece_t board[64];
			for (int i = 0; i < 64; ++i) {
				board[i] = 0;
			}
			pos.mailbox_from_bitboard(board);
			Move m = convert_move(textual_move, board);

			//TODO make this more elegant
			int from = get_from(m);
			piece_t moving = board[from];
			if (moving > 6) {
				moving = -(moving - 6);
			}
			pos.visualize_mailbox_board(board, cout);
			int to = get_to(m);
			int captured = board[to];
			//cout << "captured: " << captured << endl;
			if (captured > 6) {
				captured = -(captured - 6);
			}
			//cout << "captured: " << captured << endl;

			//cout << "from: " << from << endl;
			//cout << "moving piece: " << moving << endl;
			//cout << "make_uci_move: " << textual_move << endl;
			Move_state ms;
			ms.captured = captured;
			pos.make_move(m, ms);// m.from, m.to, m.captured, m.promoted);


	//			++Interface::Info::nodes;

				//isGivingCheck = null;
				//isReceivingCheck = null;
			pos.print(cout);
		}
		//
		static Move convert_move(string moveString, piece_t* board) {
			square_t from = static_cast<square_t> (Util::decode_square(moveString.substr(0, 2)));

			square_t to = static_cast<square_t> (Util::decode_square(moveString.substr(2, 4)));
			string promotedTo = moveString.substr(4);
			int promoted_to = 0;
			if (promotedTo != "") {
				promoted_to = static_cast<piece_t> (Util::decode_piece(promotedTo));
			}


			move_type_t mt = NONE;
			piece_t moving = board[from];
			//piece_t captured = board[to];

			// TODO stupid way of doing this
			if (moving == Piece::WHITE_PAWN) {
				if (board[to] == 0) {
					if (to - from == 9 || to - from == 7) {
						mt = EN_PASSANT;
					}
				}
				//if (to == Bitboard::extract_square(pos.en_passant_square)) {
				//	mt = EN_PASSANT;
				//}
			}
			if (moving == Piece::BLACK_PAWN) {
				if (board[to] == 0) {
					if (from - to == 9 || from - to == 7) {
						mt = EN_PASSANT;
					}
				}
				//if (to == Bitboard::extract_square(pos.en_passant_square)) {
				//	mt = EN_PASSANT;
				//}
			}
			Move m{ 0 }; //TODO
			set_from(m, from);
			set_to(m, to);
			auto is_ep = mt == EN_PASSANT;
			set_en_passant(m, is_ep);
			
//			(from, to, mt);/// , captured, false);
			//m.set_promoted_to(promoted_to);
			return m;
		}
	};
}