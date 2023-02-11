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
		
	}

	uint64_t Perft_command::perft(uint8_t depth) {
		if (depth == 0) {
			return 1;
		}
		Move_container move_container = mg.generate_pseudolegal_moves(*pp, depth);
		move_container_t moves = move_container.get_moves();
		uint64_t total_result = 0;
		size_t size = move_container.size();

		////Only works when all moves are legal
		//if (depth == 1) {
		//	return size;
		//}
		//cout << "down " << int(depth) << endl << endl;
		for (size_t i = 0; i < size; ++i) {
			Move& move = moves[i];
			Move_state ms;
			pp->make_move(move, ms);
			s.push(move);
			//cout << to_string(move) << endl;
			assert(pp->get_piece_on(get_from(move)) == 0);
			assert(pp->get_piece_on(get_to(move)) != 0);

			if (pp->is_in_check(!pp->white_to_move, &s)) {
				pp->unmake_move(move, ms);
				s.pop();
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
			assert(pp->get_piece_on(get_from(move)) == 0);
			assert(pp->get_piece_on(get_to(move)) != 0);
			pp->unmake_move(move, ms);
			s.pop();
			const bb colour = !pp->white_to_move ? pp->white : pp->black;
			const bb kpbb = pp->kings & colour;
			if (kpbb == 0) { //TODO debug flag
				cout << pp->print_bitboard(pp->kings);
				cout << pp->print_bitboard(colour);
				cout << pp->print_bitboard(pp->white);
				cout << pp->print_bitboard(pp->black);
				cout << pp->print_board();
				while (!s.empty()) {
					Move m = s.top();
					cout << to_string(m) << endl;
					s.pop();
				}

			}
			assert(kpbb != 0);
			assert(pp->get_piece_on(get_from(move)) != 0);
		}
		//cout << "up " << int(depth) << endl << endl;

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
			//"8/1K6/8/kp6/pP6/P7/8/8 b - b3 0 1";
			"rnbqkbnr/ppp2ppp/8/3pP3/8/8/PPP1PPPP/RNBQKBNR w KQkq d6 0 3";
		//"6qk/8/8/6pP/8/8/8/7K w - g6 0 3 ";
		//"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
	//"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ";
	////"k7/8/8/8/8/8/8/5B1K w - -";
	//position = Position::create_position(perft_string);
	//position = Position::create_start_position();
		cout << "Perft " << to_string(depth) << " for this position:\n";
		//		cout << perft_string << "\n" << endl;
				//cout << position << "\n";
		cout << position.print_board();
		//cout << position.debug_board();
		//position.debug_position();
		//mg.pregenerate_moves();
		clock_t begin = clock();

		///////////////////////////////
		uint64_t total_result = 0;
		pp = make_shared < Position >(position);
		Move_container move_container = mg.generate_pseudolegal_moves(position, depth);
		size_t move_count = move_container.size();

		if (depth == 0) {
			//			total_result = move_count;
			++total_result;
		}
		else {
			move_container_t moves = move_container.get_moves();

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

				uint64_t perft_result = perft(static_cast<uint8_t>(depth - 1));
				string s = to_string(move);
				cout << s << ": " << perft_result << endl;
				//pp->debug_position();
				total_result += perft_result;
				pp->unmake_move(move, ms);
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