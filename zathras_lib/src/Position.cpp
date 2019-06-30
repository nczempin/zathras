/*
 * Position.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#include <sstream>
#include <vector>
#include <iomanip>
#include <array>
#include <cassert>

#include "Position.h"
#include "Square.h"
#include "Piece.h"
#include "typedefs.h"
#include "Move_generator.h"

#include "Bitboard.h"
namespace Positions {
	Position::Position() {
		for (int i = 0; i < 6; ++i) {
			piece_bb[i] = 0;
		}
		//TODO clear piece bitboards?

	}
	//
	Position::~Position() {
		// TODO Auto-generated destructor stub
	}
	vector<string>& Position::split(const string& s, char delim, vector<string>& elems) {
		//TODO why am I both passing and returning elems? copy in between?
		stringstream ss(s);
		string item;
		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	vector<string> Position::split(const string& s, char delim) {
		vector<string> elems;
		split(s, delim, elems);
		return elems;
	}
	bool is_digit(const char c) {
		return '0' <= c && c <= '9';
	}

	void Position::set_square(bitset<64> & bs, const uint8_t to) {
		bs[to] = true;
	}

	void Position::clear_square(bitset<64> & bs, uint8_t to) {
		bs[to] = false;
	}
	void Position::set_square(bb& b, uint8_t to) {
		bb tmp = 1ULL << to;
		b |= tmp;
	}
	void Position::clear_square(bb& b, uint8_t to) {
		bb tmp = ~(1ULL << to);
		b &= tmp;
	}
	void Position::set_bit(bb& b, uint8_t to) {
		uint8_t rank = to / 8;
		uint8_t file = (to % 8);
		set_square(file, rank, b);
	}
	void Position::clear_bit(bb& b, uint8_t to) {
		uint8_t rank = to / 8;
		uint8_t file = (to % 8);
		clear_square(file, rank, b);
	}
	bool Position::is_set_square(bb b, uint8_t to) {
		uint8_t t2 = (to / 8) * 8 + (7 - (to % 8)); // mirror row
		bb ttt = 1ULL << (t2);
		bb aaa = b & ttt;
		return aaa != 0;
	}


	void Position::set_square(const uint8_t& file, const uint8_t& rank, bb& bbs) {
		uint8_t to_twisted = 7 - file + rank * 8;
		//uint8_t to = file + rank * 8;
		Position::set_square(bbs, to_twisted);
	}

	void Position::clear_square(const uint8_t& file, const uint8_t& rank, bb& bbs) {
		uint8_t to_twisted = 7 - file + rank * 8;
		//uint8_t to = file + rank * 8;
		Position::clear_square(bbs, to_twisted);
	}

	Position Position::create_position(const string& fen) {
		Position position;
		vector<string> split_fen = split(fen, ' ');
		string to_move = split_fen[1];
		string castling_string = split_fen[2];
		for (int i = 0; i < 4; ++i) {
			position.castling[i] = false;
		}
		if (castling_string != "-") {
			for (auto& right : castling_string) {
				//TODO make more elegant
				switch (right) {
				case ('K'):
					position.castling[0] = true;
					break;
				case ('Q'):
					position.castling[1] = true;
					break;
				case ('k'):
					position.castling[2] = true;
					break;
				case ('q'):
					position.castling[3] = true;
					break;
				default:
					break;
				}
				//cout << "right: " << right << endl;
			}
		}
		string en_passant = split_fen[3];
		if (en_passant != "-") {
			bb en_passant_square = 0x00;
			//TODO this will crash on invalid e.p. square string
			char file_string = en_passant[0];
			uint8_t file = file_string - 'a';
			char rank_string = en_passant[1];
			uint8_t rank = rank_string - '1';
			set_square(file, rank, en_passant_square);
			position.en_passant_square = en_passant_square;
		}

		position.white_to_move = to_move == "w" ? true : false;
		string fen_board = split_fen[0];
		vector<string> ranks = split(fen_board, '/');
		int r = 7;
		for (auto& rank : ranks) {
			int f = 0;
			for (auto& c : rank) {
				if (is_digit(c)) {
					int digit = c - '0'; //convert from ascii
					f += digit;
				}
				else {
					switch (c) {
					case 'P':
						set_square(f, r, position.pawns);
						set_square(f, r, position.white);
						break;
					case 'N':
						set_square(f, r, position.knights);
						set_square(f, r, position.white);
						break;
					case 'B':
						set_square(f, r, position.bishops);
						set_square(f, r, position.white);
						break;
					case 'R':
						set_square(f, r, position.rooks);
						set_square(f, r, position.white);
						break;
					case 'Q':
						set_square(f, r, position.queens);
						set_square(f, r, position.white);
						break;
					case 'K':
						set_square(f, r, position.kings);
						set_square(f, r, position.white);
						break;
					case 'p':
						set_square(f, r, position.pawns);
						set_square(f, r, position.black);
						break;
					case 'n':
						set_square(f, r, position.knights);
						set_square(f, r, position.black);
						break;
					case 'b':
						set_square(f, r, position.bishops);
						set_square(f, r, position.black);
						break;
					case 'r':
						set_square(f, r, position.rooks);
						set_square(f, r, position.black);
						break;
					case 'q':
						set_square(f, r, position.queens);
						set_square(f, r, position.black);
						break;
					case 'k':
						set_square(f, r, position.kings);
						set_square(f, r, position.black);
						break;
					default:
						cerr << "unknown symbol: " << c << endl;
						throw 483;
					}
					++f;

				}
			}
			--r;
		}
		return position;
	}

	Position Position::create_start_position() {
		const char* p = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

		//const char* p = "r3r1k1/1p3nqp/2pp4/p4p2/Pn3P1Q/2N4P/1PPR2P1/3R1BK1 w - - 0 1";

		Position start_position = create_position(p);
		return start_position;
	}
	ostream& operator<<(ostream& stream, const Position& position) {
		position.print(stream);
		return stream;
	}

	string Position::extract_row_string(uint_fast8_t row, string set) {

		string clear = " .";
		string retval = "";

		for (int i = 0; i < 8; ++i) {
			int tmp = row & 128;
			row = row << 1;
			string which = tmp != 0 ? set : clear;
			retval += which;
		}
		return retval;
	}

	void Position::visualize_bitboard(bb my_bb, ostream& stream) {
		stream << "  +-----------------+" << endl;
		for (int i = 7; i >= 0; --i) {
			bb tmp = (my_bb & 0xff00000000000000) >> 8 * 7; // slightly less efficient/elegant because I want the most significant byte to be on the top left
			my_bb = my_bb << 8;
			stream << (i + 1) << " |";
			string row_string = extract_row_string(tmp, " *");
			stream << row_string;
			stream << " |" << endl;
		}
		stream << "  +-----------------+" << endl;
		stream << "    a b c d e f g h" << endl;
	}
	string Position::print_bitboard(bb my_bb) {
		string retval;
		retval += "  +-----------------+\n";
		for (int i = 7; i >= 0; --i) {
			bb tmp = (my_bb & 0xff00000000000000) >> 8 * 7; // slightly less efficient/elegant because I want the most significant byte to be on the top left
			my_bb = my_bb << 8;
			retval += to_string(i + 1);
			retval += " |";
			string row_string = extract_row_string(tmp, " *");
			retval += row_string;
			retval += " |\n";
		}
		retval += "  +-----------------+\n";
		retval += "    a b c d e f g h\n";
		return retval;
	}

	void Position::visualize_mailbox_board(const int board[64], ostream& stream) {
		stream << "  +-----------------+" << endl;
		const char* symbols = ".PNBRQKpnbrqk*";
		for (int i = 0; i < 8; ++i) {
			stream << (8 - i) << " |";
			for (int j = 0; j < 8; ++j) {
				int coord = (7 - i) * 8 + j;
				stream << " " << symbols[board[coord]];
			}
			stream << " |\n";
		}
		stream << "  +-----------------+\n";
		stream << "    a b c d e f g h\n";

	}
	string Position::print_mailbox_board(const int board[64]) {
		string retval;
		retval += "  +-----------------+\n";
		const char* symbols = ".PNBRQKpnbrqk*";
		for (int i = 0; i < 8; ++i) {
			retval.append(to_string(8 - i));
			retval.append(" |");
			for (int j = 0; j < 8; ++j) {
				int coord = (7 - i) * 8 + j;
				retval.append(" ");
				retval.append(1, symbols[board[coord]]);
			}
			retval.append(" |\n");
		}
		retval.append("  +-----------------+\n");
		retval.append("    a b c d e f g h\n");
		return retval;
	}

	void Position::visit_mailbox_board(const int board[64], void (*f)(int)) {
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				int coord = (7 - i) * 8 + j;
				f(board[coord]);
			}
		}
	}

	void Position::print(ostream& stream) const {
		// TODO: Being a little inconsistent here with the types (int vs. uint_fastbla etc.)

		int board[64];
		for (int i = 0; i < 64; ++i) {
			board[i] = 0;
		}
		Bitboard::visit_bitboard(white & pawns, [&board](int x) {
			board[x] = 1;
			});
		Bitboard::visit_bitboard(white & knights, [&board](int x) {
			board[x] = 2;
			});
		Bitboard::visit_bitboard(white & bishops, [&board](int x) {
			board[x] = 3;
			});
		Bitboard::visit_bitboard(white & rooks, [&board](int x) {
			board[x] = 4;
			});
		Bitboard::visit_bitboard(white & queens, [&board](int x) {
			board[x] = 5;
			});
		Bitboard::visit_bitboard(white & kings, [&board](int x) {
			board[x] = 6;
			});
		Bitboard::visit_bitboard(black & pawns, [&board](int x) {
			board[x] = 7;
			});
		Bitboard::visit_bitboard(black & knights, [&board](int x) {
			board[x] = 8;
			});
		Bitboard::visit_bitboard(black & bishops, [&board](int x) {
			board[x] = 9;
			});
		Bitboard::visit_bitboard(black & rooks, [&board](int x) {
			board[x] = 10;
			});
		Bitboard::visit_bitboard(black & queens, [&board](int x) {
			board[x] = 11;
			});
		Bitboard::visit_bitboard(black & kings, [&board](int x) {
			board[x] = 12;
			});
		Bitboard::visit_bitboard(~(black | white), [&board](int x) {
			board[x] = 13;
			});
		visualize_mailbox_board(board, stream);
		cout << "wtm: " << white_to_move << endl;
		cout << "ep: ";
		Bitboard::visit_bitboard(en_passant_square, [](uint8_t y) {
			cout << Square::mailbox_index_to_square(y) << endl;
			});
		cout << endl;
		cout << "Castling: ";
		static char castling_chars[] = "KQkq";
		for (int i = 0; i < 4; ++i) {
			if (castling[i]) {
				cout << castling_chars[i];
			}
		}
		cout << endl;
	}

	void Position::mailbox_from_bitboard(int board[64]) const {
		Bitboard::visit_bitboard(white & pawns, [&board](int x) {
			board[x] = 1;
			}
		);
		Bitboard::visit_bitboard(white & knights, [&board](int x) {
			board[x] = 2;
			}
		);
		Bitboard::visit_bitboard(white & bishops, [&board](int x) {
			board[x] = 3;
			}
		);
		Bitboard::visit_bitboard(white & rooks, [&board](int x) {
			board[x] = 4;
			}
		);
		Bitboard::visit_bitboard(white & queens, [&board](int x) {
			board[x] = 5;
			}
		);
		Bitboard::visit_bitboard(white & kings, [&board](int x) {
			board[x] = 6;
			}
		);
		Bitboard::visit_bitboard(black & pawns, [&board](int x) {
			board[x] = 7;
			}
		);
		Bitboard::visit_bitboard(black & knights, [&board](int x) {
			board[x] = 8;
			}
		);
		Bitboard::visit_bitboard(black & bishops, [&board](int x) {
			board[x] = 9;
			}
		);
		Bitboard::visit_bitboard(black & rooks, [&board](int x) {
			board[x] = 10;
			}
		);
		Bitboard::visit_bitboard(black & queens, [&board](int x) {
			board[x] = 11;
			}
		);
		Bitboard::visit_bitboard(black & kings, [&board](int x) {
			board[x] = 12;
			}
		);
		Bitboard::visit_bitboard(~(black | white), [&board](int x) {
			board[x] = 0;
			}
		);
	}

	string Position::print_board() const {
		// TODO: Being a little inconsistent here with the types (int vs. uint_fastbla etc.)
		string retval("");
		int board[64];
		for (int i = 0; i < 64; ++i) {
			board[i] = 0;
		}
		mailbox_from_bitboard(board);
		retval += print_mailbox_board(board);
		retval += "wtm: " + to_string(white_to_move) + "\n";
		retval += "\n";
		retval += "Castling: ";
		static char castling_chars[] = "KQkq";
		for (int i = 0; i < 4; ++i) {
			if (castling[i]) {
				retval += castling_chars[i];
			}
		}
		retval += "\n";
		return retval;
	}

	void Position::display_all_moves(const bitboard_set& moves) {
		Bitboard::visit_bitboard(0xffffffffffffffff, [moves](uint8_t x) {
			Square::print_square(x);
			Position::visualize_bitboard(moves[x], cout);
			Bitboard::visit_bitboard(moves[x], [](uint8_t y) {
				Square::print_square(y);
				});
			});
	}

	array<bb, 9> Position::getPieceBitboards() const {
		array<bb, 9> retval;
		//TODO figure out what to do with [0]
		retval[0] = 0x00;
		retval[1] = pawns;
		retval[2] = knights;
		retval[3] = bishops;
		retval[4] = rooks;
		retval[5] = queens;
		retval[6] = kings;
		retval[7] = white;
		retval[8] = black;

		return retval;
	}

	static constexpr bool determine_colour(int piece)  {
		return piece > 0; // makes == 0 black, deal with it
	}

	static constexpr int8_t determine_piece(int8_t piece) {
		if (piece > 0) {
			return piece;
		}
		else {
			return -piece;
		}
	}

	void Position::update_bits(bb& colour, bb& piece, uint8_t clear, uint8_t set) { //TODO castling rights on regular rook move

		set_bit(piece, set);
		set_bit(colour, set);
		clear_bit(piece, clear);
		clear_bit(colour, clear);
	}

	void Position::save_en_passant_square(Move_state& move_state) {
		if (en_passant_square != 0x00) {
			uint8_t eps = Bitboard::extract_square(en_passant_square);
			move_state.set_en_passant_square(eps);
			en_passant_square = 0x00;
		}
	}
	void Position::restore_en_passant_square(const Move_state& move_state) {
		en_passant_square = move_state.get_en_passant_square();
	}

	void Position::promote(int8_t promoted_to, uint8_t to) {
		//TODO no idea whether this is faster or simply using absolute value is
		switch (promoted_to) {
		case Piece::WHITE_QUEEN:
		case Piece::BLACK_QUEEN:
			set_bit(queens, to);
			break;
		case Piece::WHITE_ROOK:
		case Piece::BLACK_ROOK:
			set_bit(rooks, to);
			break;
		case Piece::WHITE_BISHOP:
		case Piece::BLACK_BISHOP:
			set_bit(bishops, to);
			break;
		case Piece::WHITE_KNIGHT:
		case Piece::BLACK_KNIGHT:
			set_bit(knights, to);
			break;
		}
	}
	void Position::un_promote(int8_t promoted_to, uint8_t to) {
		switch (promoted_to) {
		case Piece::WHITE_QUEEN:
		case Piece::BLACK_QUEEN:
			clear_bit(queens, to);
			break;
		case Piece::WHITE_ROOK:
		case Piece::BLACK_ROOK:
			clear_bit(rooks, to);
			break;
		case Piece::WHITE_BISHOP:
		case Piece::BLACK_BISHOP:
			clear_bit(bishops, to);
			break;
		case Piece::WHITE_KNIGHT:
		case Piece::BLACK_KNIGHT:
			clear_bit(knights, to);
			break;
		}

	}

	void Position::handleCapture(const uint8_t& to, const int8_t& taken,
		Move_state& move_state) {
		switch (taken) {
		case Piece::WHITE_PAWN:
			clear_bit(white, to);
			clear_bit(pawns, to);
			break;
		case Piece::WHITE_KNIGHT:
			clear_bit(white, to);
			clear_bit(knights, to);
			break;
		case Piece::WHITE_BISHOP:
			clear_bit(white, to);
			clear_bit(bishops, to);
			break;
		case Piece::WHITE_ROOK:
			clear_bit(white, to);
			//TODO use constants, not magics
			if (castling[0] && to == 7) {
				castling[0] = false;
				move_state.set_cleared_kingside_castling(true);
			}
			else if (castling[1] && to == 0) {
				castling[1] = false;
				move_state.set_cleared_queenside_castling(true);
			}

			clear_bit(rooks, to);
			break;
		case Piece::WHITE_QUEEN:
			clear_bit(white, to);
			clear_bit(queens, to);
			break;
		case Piece::BLACK_PAWN:
			clear_bit(black, to);
			clear_bit(pawns, to);
			break;
		case Piece::BLACK_KNIGHT:
			clear_bit(black, to);
			clear_bit(knights, to);
			break;
		case Piece::BLACK_BISHOP:
			clear_bit(black, to);
			clear_bit(bishops, to);
			break;
		case Piece::BLACK_ROOK:
			clear_bit(black, to);
			//TODO use constants, not magics
			if (castling[2] && to == 63) {
				castling[2] = false;
				move_state.set_cleared_kingside_castling(true);
			}
			else if (castling[3] && to == 56) {
				castling[3] = false;
				move_state.set_cleared_queenside_castling(true);
			}
			clear_bit(rooks, to);
			break;
		case Piece::BLACK_QUEEN:
			clear_bit(black, to);
			clear_bit(queens, to);
			break;
		default:
			cerr << "mm??" << to_string(taken) << "\n";
			throw taken;
		}
	}

	void Position::make_move(const Move& move, Move_state& move_state) { //TODO move state as return value?
		bool set_en_passant = false;
		const uint8_t& from = move.get_from();
		const uint8_t& to = move.get_to();
		int8_t moving = 99;// move.get_moving_piece(); //TODO get piece on from square
		if (moving == 0) {

		}

		const int8_t& taken = 99;// move.get_captured(); //TODO get piece on to square
		if (taken != 0) {
			//cout << "capturing: " << taken << endl;
			handleCapture(to, taken, move_state);
		}
		if (white_to_move) {
			make_move_for_colour<true>(from, to, moving, move, move_state, set_en_passant);
		}
		else {
			make_move_for_colour<false>(from, to, moving, move, move_state, set_en_passant);

		}
		if (!set_en_passant) {
			en_passant_square = 0x00;
		}
		// TODO update 3 repetitions
		// TODO update 50 moves
		white_to_move = !white_to_move;
	}


	void Position::unmake_move(const Move& move, const Move_state& move_state) {
		uint8_t from = move.get_from();
		uint8_t to = move.get_to();
		int8_t moving = 99;//move.get_moving_piece(); //TODO get piece on from square
		restore_en_passant_square(move_state);
		white_to_move = !white_to_move;

		if (white_to_move) {
			switch (moving) {
			case Piece::WHITE_PAWN: {
				// move pawn back
				//TODO clearing can be saved when move was a capture. find out which is faster
				clear_bit(pawns, to);
				clear_bit(white, to);
				set_bit(pawns, from);
				set_bit(white, from);
/*
				int8_t promoted_to = move.get_promoted_to();
				if (promoted_to != 0) {*/
				if (is_in_back_rank_black(to)){
					piece_t promoted_to = Piece::WHITE_QUEEN; //TODO allow underpromotion
					un_promote(promoted_to, to);
				}
				else {
					// handle capturing by e. p.
					//TODO BIG TODO
					/*if (move.is_en_passant_capture()) {
						uint8_t target = to - 8;
						set_bit(en_passant_square, to);
						set_bit(pawns, target);
						set_bit(black, target);
					}*/

				}
				break;
			}
			case Piece::WHITE_KNIGHT:
				clear_bit(knights, to);
				clear_bit(white, to);
				set_bit(knights, from);
				set_bit(white, from);
				break;
			case Piece::WHITE_BISHOP:
				clear_bit(bishops, to);
				clear_bit(white, to);
				set_bit(bishops, from);
				set_bit(white, from);
				break;
			case Piece::WHITE_ROOK:
				clear_bit(rooks, to);
				clear_bit(white, to);
				set_bit(rooks, from);
				set_bit(white, from);
				if (move_state.is_cleared_kingside_castling()) {
					castling[0] = true;
				}
				if (move_state.is_cleared_queenside_castling()) {
					castling[1] = true;
				}
				break;
			case Piece::WHITE_QUEEN:
				clear_bit(queens, to);
				clear_bit(white, to);
				set_bit(queens, from);
				set_bit(white, from);
				break;
			case Piece::WHITE_KING:
				//      Position::visualize_bitboard(kings, cout);
				update_bits(white, kings, to, from);
				if (to == from - 2) { //queenside castle
					update_bits(white, rooks, 3, 0);//TODO constants, not magics
				}
				else if (from == to - 2) { // kingside castle
					update_bits(white, rooks, 5, 7);//TODO constants, not magics
				}
				if (move_state.is_cleared_kingside_castling()) {
					castling[0] = true;
				}
				if (move_state.is_cleared_queenside_castling()) {
					castling[1] = true;
				}
				break;
			default:
				cerr << "unexpected white piece: " << ((int)moving) << endl;
				throw moving;
				break;
			}
		}
		else {
			switch (moving) {
			case Piece::BLACK_PAWN: {

				clear_bit(pawns, to);
				clear_bit(black, to);
				set_bit(pawns, from);
				set_bit(black, from);
				//int8_t promoted_to = move.get_promoted_to();
				//if (promoted_to != 0) {
				if(is_in_back_rank_white(to)){
					un_promote(Piece::BLACK_QUEEN, to);
				}
				else {
					//TODO BIG TODO
					// handle capturing by e. p.
					//if (move.is_en_passant_capture()) {
					//	uint8_t target = to + 8;
					//	set_bit(en_passant_square, to);
					//	set_bit(pawns, target);
					//	set_bit(white, target);
					//	//          cout << "unmade epcap to this: " << endl << (*this) << endl;
					//}
				}
			}

									break;
			case Piece::BLACK_KNIGHT:
				clear_bit(knights, to);
				clear_bit(black, to);
				set_bit(knights, from);
				set_bit(black, from);
				break;
			case Piece::BLACK_BISHOP:
				clear_bit(bishops, to);
				clear_bit(black, to);
				set_bit(bishops, from);
				set_bit(black, from);
				break;
			case Piece::BLACK_ROOK:
				clear_bit(rooks, to);
				clear_bit(black, to);
				set_bit(rooks, from);
				set_bit(black, from);
				if (move_state.is_cleared_kingside_castling()) {
					castling[2] = true;
				}
				if (move_state.is_cleared_queenside_castling()) {
					castling[3] = true;
				}
				break;
			case Piece::BLACK_QUEEN:
				clear_bit(queens, to);
				clear_bit(black, to);
				set_bit(queens, from);
				set_bit(black, from);
				break;
			case Piece::BLACK_KING:
				update_bits(black, kings, to, from);
				if (to == from - 2) { //queenside castle
					update_bits(black, rooks, 59, 56);//TODO constants, not magics
				}
				else if (from == to - 2) { // kingside castle
					update_bits(black, rooks, 61, 63);//TODO constants, not magics
				}
				if (move_state.is_cleared_kingside_castling()) {
					castling[2] = true;
				}
				if (move_state.is_cleared_queenside_castling()) {
					castling[3] = true;
				}
				break;
			default:
				double error = ((double)moving);
				cerr << "umm: unexpected black piece: " << error << endl;
				throw - moving;
				break;
			}
		}

		//TODOif (!move.is_en_passant_capture()) {
		{
		int8_t captured = move_state.captured;// get_captured();
			if (captured != 0) {
				//cout << "untaken: " << (int)captured << endl;
				bool colour = determine_colour(captured);
				if (colour) {
					set_bit(white, to);
				}
				else {
					set_bit(black, to);
				}
				int8_t p = determine_piece(captured);
				switch (p) {
				case 1:
					set_bit(pawns, to);
					break;
				case 2:
					set_bit(knights, to);
					break;
				case 3:
					set_bit(bishops, to);
					break;
				case Piece::ROOK:
					//TODO use constants, not magics
					if (move_state.is_cleared_kingside_castling() && to == 63) {
						castling[2] = true;
					}
					else if (move_state.is_cleared_queenside_castling()) {
						castling[3] = true;
					}
					else if (move_state.is_cleared_kingside_castling() && to == 7) {
						castling[0] = true;
					}
					else if (move_state.is_cleared_queenside_castling() && to == 0) {
						castling[1] = true;
					}
					set_bit(rooks, to);
					break;
				case 5:
					set_bit(queens, to);
					break;
				case 6:
					set_bit(kings, to);
					break;
				default:
					cerr << "un??" << p << endl;
					throw p;
				}
			}
		}
		// TODO update 3 repetitions
		// TODO update 50 moves
	}

	void Position::debugPosition() {
		cout << print_board();
		cout << print_bitboard(white);
		cout << print_bitboard(black);
		cout << print_bitboard(pawns);
		for (uint8_t i = 0; i < 64; ++i) {
			if (is_set_square(black, i)) {
				cout << "1";
			}
			else {
				cout << "0";
			}
			if (i % 8 == 7) {
				cout << "\n";
			}
		}
		cout << "\n";
		for (uint8_t i = 0; i < 64; ++i) {
			if (is_set_square(white, i)) {
				cout << "1";
			}
			else {
				cout << "0";
			}
			if (i % 8 == 7) {
				cout << "\n";
			}
		}
	}


	bool Position::is_in_check(const bool side) {
		//TODO this is a somewhat naive way of doing this, it needs to be much more efficient
		const bb colour = side ? white : black;
		const bb kpbb = kings & colour;
		const uint8_t& king_pos = Bitboard::extract_square(kpbb);

		if (white_to_move) {
			const bb white_knights = knights & white;
			if (is_attacked_by_hopper(white_knights, Bitboard::knight_moves, king_pos)) {
				return true;
			}
			const bb white_pawns = pawns & white;
			if (is_attacked_by_hopper(white_pawns, Bitboard::black_pawn_capture_moves,
				king_pos)) {
				return true;
			}
			const bb occupied = black | white;
			if (is_check_from_slider(Bitboard::bishop_moves, king_pos, white & (bishops | queens),
				occupied)) {
				return true;
			}
			if (is_check_from_slider(Bitboard::rook_moves, king_pos, white & (rooks | queens), occupied)) {
				return true;
			}
			if (is_attacked_by_hopper(kings & white, Bitboard::king_moves, king_pos)) {
				return true;
			}
		}
		else {
			const bb black_knights = knights & black;
			if (is_attacked_by_hopper(black_knights, Bitboard::knight_moves, king_pos)) {
				return true;
			}
			const bb black_pawns = pawns & black;
			if (is_attacked_by_hopper(black_pawns, Bitboard::white_pawn_capture_moves,
				king_pos)) {
				return true;
			}
			const bb occupied = black | white;
			if (is_check_from_slider(Bitboard::bishop_moves, king_pos, black & (bishops | queens),
				occupied)) {
				return true;
			}
			if (is_check_from_slider(rook_moves, king_pos, black & (rooks | queens), occupied)) {
				return true;
			}
			if (is_attacked_by_hopper(kings & black, Bitboard::king_moves, king_pos)) {
				return true;
			}
		}
		return false;
	}

	bool Position::is_attacked_by_slider(bb position,
		const bitboard_set& all_moves, const uint8_t& square,
		const bb& occupied) {
		while (position != 0) {
			const uint8_t& from = Bitboard::extract_and_remove_square(position);
			const bb& raw_moves = all_moves[from];
			bb kpsq = 0;
			Position::set_bit(kpsq, square);
			bb moves = raw_moves & kpsq;
			while (moves != 0x00) {
				const uint8_t& to = Bitboard::extract_and_remove_square(moves);
				const bool& b = is_anything_between(from, to, occupied);
				if (!b) {
					return true;
				}
			}
		}
		return false;
	}


	bool Position::is_check_from_slider(const bitboard_set& sliding_moves,
		const uint8_t& king_pos, const bb& slider, const bb& occupied) {
		bool retval = false;
		const bb& raw_moves = sliding_moves[king_pos];
		const bb& moves = raw_moves & slider;
		if (moves != 0) {
			bb tmp = moves;
			uint8_t attacker = 0;
			uint8_t l = 0;
			while (tmp) {
				l = Bitboard::ffs(tmp);
				attacker = Bitboard::look_up(l);
				if (!is_anything_between(king_pos, attacker, occupied)) {
					return true;
				}
				tmp &= tmp - 1ULL; //clear LS1B
			}
		}
		return retval;
	}
	bool Position::is_attacked_by_hopper(const bb& movers,
		const bitboard_set& all_moves, const uint8_t& square) {
		const bb& raw_moves = all_moves[square];
		const bb moves = raw_moves & movers;
		return moves != 0x00;
	}

	bool Position::is_anything_between(uint8_t from, uint8_t to,
		const bb& occupied) {
		uint16_t index = calc_index(from, to);
		return between[index] & occupied;
	}

	bb Position::between[BETWEEN_ARRAY_SIZE];
	const bitboard_set Position::rook_moves = Move_generator::pregenerate_rook_moves();

}