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
#include <iomanip>

#include "Position.h"
#include "Move_generator.h"
#include <cassert>

namespace Interface {
	using namespace Moves;

	struct PerftMetrics {
		uint64_t nodes = 0;
		uint64_t captures = 0;
		uint64_t en_passant = 0;
		uint64_t castles = 0;
		uint64_t promotions = 0;
		uint64_t checks = 0;
		uint64_t discovery_checks = 0;
		uint64_t double_checks = 0;
		uint64_t checkmates = 0;
	};

	Perft_command::~Perft_command() {
		// TODO Auto-generated destructor stub
	}

	uint64_t Perft_command::perft(uint8_t depth) {
		PerftMetrics metrics;
		perft_with_metrics(depth, metrics);
		return metrics.nodes;
	}

	void Perft_command::perft_with_metrics(uint8_t depth, PerftMetrics& metrics) {
		if (depth == 0) {
			metrics.nodes = 1;
			return;
		}
		Move_container move_container = mg.generate_pseudolegal_moves(*pp, depth);
		move_container_t moves = move_container.get_moves();
		size_t size = move_container.size();

		for (size_t i = 0; i < size; ++i) {
			Move& move = moves[i];
			Move_state ms;
			
			// Save state before move for checks detection
			bool was_in_check = pp->is_in_check(pp->white_to_move);
			
			pp->make_move(move, ms);
			assert(pp->get_piece_on(move.get_from()) == 0);
			assert(pp->get_piece_on(move.get_to()) != 0);

			if (pp->is_in_check(!pp->white_to_move)) {
				pp->unmake_move(move, ms);
				continue;
			}

			// the move was legal
			if (depth == 1) {
				metrics.nodes++;
				
				// Count move types
				if (ms.captured_piece != 0) {
					metrics.captures++;
				}
				
				Move::move_t move_type = move.get_move_type();
				if (move_type == Move::EN_PASSANT) {
					metrics.en_passant++;
					metrics.captures++; // En passant is also a capture
				} else if (move_type == Move::CASTLE_KINGSIDE || move_type == Move::CASTLE_QUEENSIDE) {
					metrics.castles++;
				} else if (move_type == Move::PROMOTION_QUEEN || move_type == Move::PROMOTION_ROOK ||
				           move_type == Move::PROMOTION_BISHOP || move_type == Move::PROMOTION_KNIGHT) {
					metrics.promotions++;
				}
				
				// Check if opponent is in check or checkmate
				bool gives_check = pp->is_in_check(pp->white_to_move);
				if (gives_check) {
					metrics.checks++;
					
					// Check if it's checkmate
					Move_container check_moves = mg.generate_pseudolegal_moves(*pp, 1);
					bool has_legal_move = false;
					for (size_t j = 0; j < check_moves.size(); ++j) {
						Move_state temp_ms;
						pp->make_move(check_moves.get_moves()[j], temp_ms);
						if (!pp->is_in_check(!pp->white_to_move)) {
							has_legal_move = true;
							pp->unmake_move(check_moves.get_moves()[j], temp_ms);
							break;
						}
						pp->unmake_move(check_moves.get_moves()[j], temp_ms);
					}
					if (!has_legal_move) {
						metrics.checkmates++;
					}
				}
			}
			else {
				PerftMetrics sub_metrics;
				perft_with_metrics(depth - 1, sub_metrics);
				metrics.nodes += sub_metrics.nodes;
				metrics.captures += sub_metrics.captures;
				metrics.en_passant += sub_metrics.en_passant;
				metrics.castles += sub_metrics.castles;
				metrics.promotions += sub_metrics.promotions;
				metrics.checks += sub_metrics.checks;
				metrics.discovery_checks += sub_metrics.discovery_checks;
				metrics.double_checks += sub_metrics.double_checks;
				metrics.checkmates += sub_metrics.checkmates;
			}
			assert(pp->get_piece_on(move.get_from()) == 0);
			assert(pp->get_piece_on(move.get_to()) != 0);
			pp->unmake_move(move, ms);
			assert(pp->get_piece_on(move.get_from()) != 0);
		}
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
		//position.debugPosition();
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

				uint64_t perft_result = perft(depth - 1);
				string s = to_string(move);
				cout << s << ": " << perft_result << endl;
				//pp->debugPosition();
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