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
#include <cassert>

namespace Interface {
	using namespace Moves;


	Perft_command::~Perft_command() {
		// TODO Auto-generated destructor stub
	}

	uint64_t Perft_command::perft(uint8_t depth) {
		if (depth == 0) {
			return 1;
		}

		Move hurz[127]; //TODO
		Move* moves = hurz; //TODO
		moves = mg.generate_pseudolegal_moves(*pp, moves);
		uint64_t total_result = 0;
		size_t size = moves - hurz; //TODO

		////Only works when all moves are legal
		if (depth == 1) {
			return size;
		}

		for (Move* move = hurz; move != moves; move++) {
//			Move& move = hurz[i];
			Move_state ms;
			pp->make_move(*move, ms);
			assert(pp->get_piece_on(get_from(*move)) == 0);
			assert(pp->get_piece_on(get_to(*move)) != 0);

	/*		if (pp->is_in_check(!pp->white_to_move)) {
				pp->unmake_move(move, ms);
				continue;
			}*/

			// the move was legal
			if (depth == 1) {
				++total_result;
			}
			else {
				uint64_t perft_result = perft(depth - 1);
				total_result += perft_result;
			}
			assert(pp->get_piece_on(get_from(*move)) == 0);
			assert(pp->get_piece_on(get_to(*move)) != 0);
			pp->unmake_move(*move, ms);
			assert(pp->get_piece_on(get_from(*move)) != 0);
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
	
		cout << "Perft " << to_string(depth) << " for this position:\n";
		cout << position.print_board();
		clock_t begin = clock();

		///////////////////////////////
		uint64_t total_result = 0;
		pp = make_shared < Position >(position);

		Move hurz[127]; //TODO
		Move* moves = hurz; //TODO
		moves = mg.generate_pseudolegal_moves(*pp, moves);
		
		size_t size = moves - hurz; //TODO

		size_t move_count = size;
		

		if (depth == 1) {
						total_result = move_count;
			//++total_result;
		}
		else {

			for (Move* move = hurz; move != moves; move++) {
//				Move& move = hurz[i];
				Move_state ms;
				pp->make_move(*move, ms);
//				mg.outside = true;
				//if (pp->is_in_check(!pp->white_to_move)) {
				//	//++illegal_moves_generated;
				//	pp->unmake_move(move, ms);
				//	continue;
				//}

				uint64_t perft_result = perft(depth - 1);
				string s = move_to_string(*move);
				cout << s << ": " << perft_result << endl;
				//pp->debug_position();
				total_result += perft_result;
				pp->unmake_move(*move, ms);
				//pp->debug_position();

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