/*
 * Perft_command.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#include "Perft_command.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <ctime>

#include "Position.h"
#include "Move_generator.h"

namespace Interface {
	using namespace Moves;
	

	Perft_command::~Perft_command() {
		// TODO Auto-generated destructor stub
	}

	uint64_t Perft_command::perft(uint8_t depth) {
		if (depth == 0) {
			return 1;
		}
		Move_container move_container = mg.generate_pseudolegal_moves(*pp, depth);
		array<Move, Move_container::SIZE> moves = move_container.get_moves();
		uint64_t total_result = 0;
		size_t size = move_container.size();
		for (size_t i = 0; i < size; ++i) {
			Move& move = moves[i];
			Move_state ms;
			pp->make_move(move, ms);
			if (pp->is_in_check(!pp->white_to_move)) {
				//++illegal_moves_generated;
				pp->unmake_move(move, ms);
				continue;
			}
			// the move was legal
			if (depth == 1) {
				++total_result;
			}
			else {
				uint64_t perft_result = perft(depth - 1);
				total_result += perft_result;
			}
			pp->unmake_move(move, ms);
		}
		return total_result;
	}

	string Perft_command::format_large_number(int nps) {
		string nps_wc = to_string(nps);
		int8_t insert_position = static_cast<int8_t>(nps_wc.length()) - 3;
		while (insert_position > 0) {
			nps_wc.insert(insert_position, ",");
			insert_position -= 3;
		}
		return nps_wc;
	}

	void Perft_command::execute() {
		//vector<string> path = receiver->getArguments();
		//uint8_t depth = 6; //TODO get this from arguments, but use a reasonable default
		//Position position;
		const string perft_string =
			"8/1K6/8/kp6/pP6/P7/8/8 b - b3 0 1";
			//"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
		//"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ";
		////"k7/8/8/8/8/8/8/5B1K w - -";
		position = Position::create_position(perft_string);
		//position = Position::create_start_position();
		cout << "Perft " << to_string(depth) << " for this position:\n";
//		cout << perft_string << "\n" << endl;
		//cout << position << "\n";
		cout << position.print_board();
		//mg.pregenerate_moves();
		clock_t begin = clock();

		///////////////////////////////
		uint64_t total_result = 0;
		pp = make_shared < Position >(position);
		Move_container move_container = mg.generate_pseudolegal_moves(position, depth);
		size_t move_count = move_container.size();

		if (depth == 0) {
			total_result = move_count;
		}
		else {
			array<Move, Move_container::SIZE> moves = move_container.get_moves();

			for (size_t i = 0; i < move_count; ++i) {
				Move& move = moves[i];
				Move_state ms;
				pp->make_move(move, ms);
				mg.outside = true;
				if (pp->is_in_check(!pp->white_to_move)) {
					//++illegal_moves_generated;
					pp->unmake_move(move, ms);
					continue;
				}

				uint64_t perft_result = perft(depth - 1);
				string s = to_string(move);
				cout << s << ": " << perft_result << endl;
				total_result += perft_result;
				pp->unmake_move(move, ms);
			}
		}

		clock_t end = clock();
		double elapsed_secs = (static_cast<double>(end) - static_cast<double>(begin)) / CLOCKS_PER_SEC;
		int nps = static_cast<int>(total_result / elapsed_secs);
		//	string nps_wc = format_large_number(nps);
		cout << endl << "Perft " << to_string(depth) << " result: " << total_result << endl;
		//cout << "(illegal moves generated)" << illegal_moves_generated << endl;
		//cout.imbue(locale(""));
		cout << "time: " << elapsed_secs << ", nps: " << nps << endl;
	}
}