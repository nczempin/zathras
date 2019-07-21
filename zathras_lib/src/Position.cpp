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

		//TODO statically initialize


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
			Square::set_square(file, rank, en_passant_square);
			position.en_passant_square = en_passant_square;
		}

		position.white_to_move = to_move == "w" ? true : false;
		string fen_board = split_fen[0];
		vector<string> ranks = split(fen_board, '/');
		int r = 7;
		fill_n(position.board, 64, 0);
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
						Square::set_square(f, r, position.pawns);
						Square::set_square(f, r, position.white);
						position.board[f + r * 8] = Piece::WHITE_PAWN; //TODO encapsulate
						break;
					case 'N':
						Square::set_square(f, r, position.knights);
						Square::set_square(f, r, position.white);
						position.board[f + r * 8] = Piece::WHITE_KNIGHT; //TODO encapsulate
						break;
					case 'B':
						Square::set_square(f, r, position.bishops);
						Square::set_square(f, r, position.white);
						position.board[f + r * 8] = Piece::WHITE_BISHOP; //TODO encapsulate
						break;
					case 'R':
						Square::set_square(f, r, position.rooks);
						Square::set_square(f, r, position.white);
						position.board[f + r * 8] = Piece::WHITE_ROOK; //TODO encapsulate
						break;
					case 'Q':
						Square::set_square(f, r, position.queens);
						Square::set_square(f, r, position.white);
						position.board[f + r * 8] = Piece::WHITE_QUEEN; //TODO encapsulate
						break;
					case 'K':
						Square::set_square(f, r, position.kings);
						Square::set_square(f, r, position.white);
						position.board[f + r * 8] = Piece::WHITE_KING; //TODO encapsulate
						break;
					case 'p':
						Square::set_square(f, r, position.pawns);
						Square::set_square(f, r, position.black);
						position.board[f + r * 8] = Piece::BLACK_PAWN; //TODO encapsulate
						break;
					case 'n':
						Square::set_square(f, r, position.knights);
						Square::set_square(f, r, position.black);
						position.board[f + r * 8] = Piece::BLACK_KNIGHT; //TODO encapsulate
						break;
					case 'b':
						Square::set_square(f, r, position.bishops);
						Square::set_square(f, r, position.black);
						position.board[f + r * 8] = Piece::BLACK_BISHOP; //TODO encapsulate
						break;
					case 'r':
						Square::set_square(f, r, position.rooks);
						Square::set_square(f, r, position.black);
						position.board[f + r * 8] = Piece::BLACK_ROOK; //TODO encapsulate
						break;
					case 'q':
						Square::set_square(f, r, position.queens);
						Square::set_square(f, r, position.black);
						position.board[f + r * 8] = Piece::BLACK_QUEEN; //TODO encapsulate
						break;
					case 'k':
						Square::set_square(f, r, position.kings);
						Square::set_square(f, r, position.black);
						position.board[f + r * 8] = Piece::BLACK_KING; //TODO encapsulate
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

	void Position::visualize_mailbox_board(const piece_t board[64], ostream& stream) {
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
	string Position::print_mailbox_board(const piece_t board[64]) {
		string retval;
		retval += "  +-----------------+\n";
		const char* symbols = ".pnrbqkPNBRQK*";
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
	string Position::debug_mailbox_board(const piece_t board[64]) {
		string retval;
		retval += "  +-----------------+\n";
		const char* symbols = ".PNBRQKpnbrqk*";
		for (int i = 0; i < 8; ++i) {
			retval.append(to_string(8 - i));
			retval.append(" |");
			for (int j = 0; j < 8; ++j) {
				int coord = (7 - i) * 8 + j;
				retval.append(" ");
				retval.append(to_string(int(board[coord])));
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

		piece_t board[64];
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

	void Position::mailbox_from_bitboard(piece_t board[64]) const {
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

	string Position::debug_board() const {
		// TODO: Being a little inconsistent here with the types (int vs. uint_fastbla etc.)
		string retval("");
		retval += debug_mailbox_board(board);
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

	string Position::print_board() const {
		// TODO: Being a little inconsistent here with the types (int vs. uint_fastbla etc.)
		string retval("");
		piece_t mboard[64];
		for (int i = 0; i < 64; ++i) {
			mboard[i] = 0;
		}
		mailbox_from_bitboard(mboard);
		retval += print_mailbox_board(mboard);
		retval += debug_mailbox_board(board);
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

	static constexpr bool determine_colour(int piece) {
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



	inline void Position::save_en_passant_square(Move_state& move_state) {
		if (en_passant_square != 0x00) {
			uint8_t eps = Bitboard::extract_square(en_passant_square);
			move_state.set_en_passant_square(eps);
			en_passant_square = 0x00;
		}
	}
	void Position::restore_en_passant_square(const Move_state& move_state) {
		en_passant_square = move_state.get_en_passant_square();
	}

	void Position::promote(int8_t promoted_to, square_t to) {
		//TODO no idea whether this is faster or simply using absolute value is
		switch (promoted_to) {
		case Piece::WHITE_QUEEN:
		case Piece::BLACK_QUEEN:
			Square::set_bit(queens, to);
			break;
		case Piece::WHITE_ROOK:
		case Piece::BLACK_ROOK:
			Square::set_bit(rooks, to);
			break;
		case Piece::WHITE_BISHOP:
		case Piece::BLACK_BISHOP:
			Square::set_bit(bishops, to);
			break;
		case Piece::WHITE_KNIGHT:
		case Piece::BLACK_KNIGHT:
			Square::set_bit(knights, to);
			break;
		}
	}
	void Position::un_promote(int8_t promoted_to, square_t to) {
		switch (promoted_to) {
		case Piece::WHITE_QUEEN:
		case Piece::BLACK_QUEEN:
			Square::clear_bit(queens, to);
			break;
		case Piece::WHITE_ROOK:
		case Piece::BLACK_ROOK:
			Square::clear_bit(rooks, to);
			break;
		case Piece::WHITE_BISHOP:
		case Piece::BLACK_BISHOP:
			Square::clear_bit(bishops, to);
			break;
		case Piece::WHITE_KNIGHT:
		case Piece::BLACK_KNIGHT:
			Square::clear_bit(knights, to);
			break;
		}

	}

	void Position::handleCapture(const square_t& to, const int8_t& taken,
		Move_state& move_state) {
		move_state.captured = taken;
		switch (taken) {
		case Piece::WHITE_PAWN:
			Square::clear_bit(white, to);
			Square::clear_bit(pawns, to);
			break;
		case Piece::WHITE_KNIGHT:
			Square::clear_bit(white, to);
			Square::clear_bit(knights, to);
			break;
		case Piece::WHITE_BISHOP:
			Square::clear_bit(white, to);
			Square::clear_bit(bishops, to);
			break;
		case Piece::WHITE_ROOK:
			Square::clear_bit(white, to);
			//TODO use constants, not magics
			if (castling[0] && to == 7) {
				castling[0] = false;
				move_state.set_cleared_kingside_castling(true);
			}
			else if (castling[1] && to == 0) {
				castling[1] = false;
				move_state.set_cleared_queenside_castling(true);
			}

			Square::clear_bit(rooks, to);
			break;
		case Piece::WHITE_QUEEN:
			Square::clear_bit(white, to);
			Square::clear_bit(queens, to);
			break;
		case Piece::WHITE_KING:
			Square::clear_bit(white, to);
			Square::clear_bit(kings, to);
			break;
		case Piece::BLACK_PAWN:
			Square::clear_bit(black, to);
			Square::clear_bit(pawns, to);
			break;
		case Piece::BLACK_KNIGHT:
			Square::clear_bit(black, to);
			Square::clear_bit(knights, to);
			break;
		case Piece::BLACK_BISHOP:
			Square::clear_bit(black, to);
			Square::clear_bit(bishops, to);
			break;
		case Piece::BLACK_ROOK:
			Square::clear_bit(black, to);
			//TODO use constants, not magics
			if (castling[2] && to == 63) {
				castling[2] = false;
				move_state.set_cleared_kingside_castling(true);
			}
			else if (castling[3] && to == 56) {
				castling[3] = false;
				move_state.set_cleared_queenside_castling(true);
			}
			Square::clear_bit(rooks, to);
			break;
		case Piece::BLACK_QUEEN:
			Square::clear_bit(black, to);
			Square::clear_bit(queens, to);
			break;
		case Piece::BLACK_KING:
			Square::clear_bit(black, to);
			Square::clear_bit(kings, to);
			break;
		default:
			cerr << "mmc??" << to_string(taken) << "\n";
			throw taken;
		}
	}

	void Position::make_move(const Move& move, Move_state& move_state) { //TODO move state as return value?
		bool set_en_passant = false;
		const square_t& from = move.get_from();
		const square_t& to = move.get_to();
		int8_t moving = get_piece_on(from);
		//if (moving == 0) {
		//	debugPosition();
		//}
		assert(moving != 0);

		const int8_t& taken = get_piece_on(to);
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
		//TODO such a mess
		square_t from = move.get_from();
		square_t to = move.get_to();
		piece_t moving = get_piece_on(to);
		assert(moving != 0);

		board[from] = moving;
		restore_en_passant_square(move_state);
		white_to_move = !white_to_move;

		if (white_to_move) {
			Square::clear_bit(white, to);
			Square::set_bit(white, from);
			bb& pbb = piece_bb[moving - 1];
			Square::clear_bit(pbb, to);
			Square::set_bit(pbb, from);
			switch (moving) {
			case Piece::WHITE_PAWN: {
				// move pawn back
				//TODO clearing can be saved when move was a capture. find out which is faster
				if (is_in_back_rank_black(to)) {
					//TODO move_type = promotion
					//piece_t promoted_to = Piece::WHITE_QUEEN; //TODO allow underpromotion
					//un_promote(promoted_to, to);
					Square::clear_bit(queens, to);
				}
				else {
					// handle capturing by e. p.
					if (move.get_move_type() == EN_PASSANT) {
						square_t target = square_t(to - 8); //TODO
						en_passant_square = 0;
						Square::set_bit(en_passant_square, to);
						Square::set_bit(pawns, target);
						Square::set_bit(black, target);
						board[target] = Piece::BLACK_PAWN;
						board[to] = 0;
					}

				}
				break;
			}
			case Piece::WHITE_KNIGHT:
			case Piece::WHITE_BISHOP:
			case Piece::WHITE_QUEEN:
				break;
			case Piece::WHITE_ROOK:
				if (move_state.is_cleared_kingside_castling()) {
					castling[0] = true;
				}
				if (move_state.is_cleared_queenside_castling()) {
					castling[1] = true;
				}
				break;
			case Piece::WHITE_KING:
				if (from == E1 && to == C1) { //queenside castle
					Square::update_bits(white, rooks, D1, A1);
					board[D1] = 0;
					board[A1] = Piece::WHITE_ROOK;
				}
				else if (from == E1 && to == G1) { // kingside castle
					Square::update_bits(white, rooks, F1, H1);
					board[F1] = 0;
					board[H1] = Piece::WHITE_ROOK;
				}
				if (move_state.is_cleared_kingside_castling()) {
					castling[0] = true;
				}
				if (move_state.is_cleared_queenside_castling()) {
					castling[1] = true;
				}
				break;
			default:
				debugPosition();
				cerr << "unexpected white piece: " << ((int)moving) << endl;
				throw moving;
				break;
			}
		}
		else {
			Square::clear_bit(black, to); //TODO unnecessary when capture
			Square::set_bit(black, from);
			bb& pbb = piece_bb[-moving - 1];
			Square::clear_bit(pbb, to); //TODO clear and set in one op when capture?
			Square::set_bit(pbb, from);
			switch (moving) {
			case Piece::BLACK_PAWN: {

				if (is_in_back_rank_white(to)) {
					//un_promote(Piece::BLACK_QUEEN, to);
					Square::clear_bit(queens, to);
				}
				else {
					// handle capturing by e. p.
					if (move.get_move_type() == EN_PASSANT) {
						square_t target = square_t(to + 8); //TODO
						en_passant_square = 0;
						Square::set_bit(en_passant_square, to);
						Square::set_bit(pawns, target);
						Square::set_bit(white, target);
						board[target] = Piece::WHITE_PAWN;
						board[to] = 0;
					}
				}
			}

									break;
			case Piece::BLACK_KNIGHT:
			case Piece::BLACK_BISHOP:
			case Piece::BLACK_QUEEN:
				break;
			case Piece::BLACK_ROOK:
				if (move_state.is_cleared_kingside_castling()) {
					castling[2] = true;
				}
				if (move_state.is_cleared_queenside_castling()) {
					castling[3] = true;
				}
				break;
			case Piece::BLACK_KING:
				if (from == E8 && to == C8) { //queenside castle
					Square::update_bits(black, rooks, D8, A8);//TODO constants, not magics
					board[D8] = 0;
					board[A8] = Piece::BLACK_ROOK;
				}
				else if (from == E8 && to == G8) { // kingside castle
					Square::update_bits(black, rooks, F8, H8);//TODO constants, not magics
					board[F8] = 0;
					board[H8] = Piece::BLACK_ROOK;
				}
				if (move_state.is_cleared_kingside_castling()) {
					castling[2] = true;
				}
				if (move_state.is_cleared_queenside_castling()) {
					castling[3] = true;
				}
				break;
			default:
				debugPosition();
				double error = ((double)moving);
				cerr << "umm: unexpected black piece: " << error << endl;
				throw - moving;
				break;
			}
		}
		assert(get_piece_on(move.get_from()) != 0);
		if (move.get_move_type() != EN_PASSANT) {

			int8_t captured = move_state.captured;// get_captured();
			//board[from] = moving; //TODO encapsulate
			board[to] = captured; //TODO encapsulate

			if (captured != 0) {
				//cout << "untaken: " << (int)captured << endl;
				bool colour = determine_colour(captured);
				if (colour) {
					Square::set_bit(white, to);
				}
				else {
					Square::set_bit(black, to);
				}
				int8_t p = determine_piece(captured);
				switch (p) {
				case 1:
					Square::set_bit(pawns, to);
					break;
				case 2:
					Square::set_bit(knights, to);
					break;
				case 3:
					Square::set_bit(bishops, to);
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
					Square::set_bit(rooks, to);
					break;
				case 5:
					Square::set_bit(queens, to);
					break;
				case 6:
					Square::set_bit(kings, to);
					break;
				default:
					cerr << "un??" << p << endl;
					throw p;
				}
				assert(get_piece_on(move.get_from()) != 0);
				assert(get_piece_on(move.get_to()) != 0);
			}
		}


		assert(get_piece_on(move.get_from()) != 0);
		// TODO update 3 repetitions
		// TODO update 50 moves
	}

	void Position::debugPosition() {
		//for (int i = 0; i < 64; ++i) {
		//	mboard[i] = 0;
		//}

		cout << print_board();
		cout << print_bitboard(white);
		cout << print_bitboard(black);
		cout << print_bitboard(pawns);
		for (uint8_t i = 0; i < 64; ++i) {
			if (Square::is_set_square(black, square_t(i))) {
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
			if (Square::is_set_square(white, square_t(i))) {
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
		if (kpbb == 0) {
			cout << print_bitboard(kings);
			cout << print_bitboard(colour);
			cout << print_bitboard(white);
			cout << print_bitboard(black);
		}
		assert(kpbb != 0);
		const square_t& king_pos = square_t(Bitboard::extract_square(kpbb));
		assert(king_pos < 64);
		if (king_pos >= 64) {
			debugPosition();
			throw king_pos;
		}
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
		const bitboard_set& all_moves, const square_t& square,
		const bb& occupied) {
		while (position != 0) {
			const square_t& from = square_t(Bitboard::extract_and_remove_square(position));
			const bb& raw_moves = all_moves[from];
			bb kpsq = 0;
			Square::set_bit(kpsq, square);
			bb moves = raw_moves & kpsq;
			while (moves != 0x00) {
				const square_t& to = square_t(Bitboard::extract_and_remove_square(moves));
				const bool& b = is_anything_between(from, to, occupied);
				if (!b) {
					return true;
				}
			}
		}
		return false;
	}


	bool Position::is_check_from_slider(const bitboard_set& sliding_moves,
		const square_t& king_pos, const bb& slider, const bb& occupied) {
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
				if (!is_anything_between(king_pos, square_t(attacker), occupied)) {
					return true;
				}
				tmp &= tmp - 1ULL; //clear LS1B
			}
		}
		return retval;
	}
	bool Position::is_attacked_by_hopper(const bb& movers,
		const bitboard_set& all_moves, const square_t& square) {
		const bb& raw_moves = all_moves[square];
		const bb moves = raw_moves & movers;
		return moves != 0x00;
	}

	bool Position::is_anything_between(square_t from, square_t to,
		const bb& occupied) {
		uint16_t index = calc_index(from, to);
		return between[index] & occupied;
	}

	bb Position::between[BETWEEN_ARRAY_SIZE];
	const bitboard_set Position::rook_moves = Move_generator::pregenerate_rook_moves();
	template<bool white_or_not>
	inline void Position::make_move_for_colour(const square_t& from, const square_t& to, const int8_t& moving, const Move& move, Move_state& move_state, bool& set_en_passant) {
		{
			board[from] = 0; //TODO encapsulate
			board[to] = moving; //TODO encapsulate
			assert(moving != 0);
			if (white_or_not) {
				Square::clear_bit(white, from);
				Square::set_bit(white, to);
				bb& pbb = piece_bb[moving - 1];
				Square::set_bit(pbb, to);
				Square::clear_bit(pbb, from);

				switch (moving) {
				case Piece::WHITE_PAWN: {
					//clear_bit(pawns, from);

					if (is_in_back_rank_black(to)) { // target is rank 8 -> promote
													 //promote(Piece::WHITE_QUEEN, to);
						Square::set_bit(queens, to);
						Square::clear_bit(pawns, to);
						board[to] = Piece::WHITE_QUEEN;
					}
					else {
						// handle capturing by e. p.

						//TODO: find out which method is faster
						//if (get_piece_on(to) == 0xff) { // en passant capture
						if (move.get_move_type() == EN_PASSANT) { // en passant capture
							square_t target = square_t(to - 8);
							Square::clear_bit(pawns, target);
							Square::clear_bit(black, target);
							board[target] = 0;
						}
						else {
							save_en_passant_square(move_state);
							// handle double step preparing the e. p.
							if (to - from == 16) {
								square_t target = square_t(to - 8);
								set_en_passant = true;
								Square::set_bit(en_passant_square, target);
							}
						}
					}
					break;
				}
				case Piece::WHITE_KNIGHT:
				case Piece::WHITE_BISHOP:
				case Piece::WHITE_QUEEN:
				{

					save_en_passant_square(move_state);
					break;
				}
				case Piece::WHITE_ROOK:
					if (from == 7 && castling[0]) { // H1 //TODO
						castling[0] = false;
						move_state.set_cleared_kingside_castling(true);
					}
					else if (from == 0 && castling[1]) { //A1 // TODO
						castling[1] = false;
						move_state.set_cleared_queenside_castling(true);
					}
					save_en_passant_square(move_state);
					break;
				case Piece::WHITE_KING:
					if (from == E1 && to == C1) { //queenside castle
						Square::update_bits(white, rooks, A1, D1);
						board[A1] = 0;
						board[D1] = Piece::WHITE_ROOK;
					}
					else if (from == E1 && to == G1) { // kingside castle
						Square::update_bits(white, rooks, H1, F1);
						board[H1] = 0;
						board[F1] = Piece::WHITE_ROOK;
					}
					if (castling[0]) {
						move_state.set_cleared_kingside_castling(true);
						castling[0] = false;
					}
					if (castling[1]) {
						move_state.set_cleared_queenside_castling(true);
						castling[1] = false;
					}
					save_en_passant_square(move_state);
					break;
				}
			}
			else {
				Square::clear_bit(black, from);
				Square::set_bit(black, to);
				bb& pbb = piece_bb[-moving - 1];
				Square::set_bit(pbb, to);
				Square::clear_bit(pbb, from);
				switch (moving) {
				case Piece::BLACK_PAWN: {
					//clear_bit(pawns, from);
					/*int8_t promoted_to = move.get_promoted_to();
					if (promoted_to != 0) {*/
					if (is_in_back_rank_white(to)) {
						Square::set_bit(queens, to);
						Square::clear_bit(pawns, to); // that was eagerly set previously
													  //							promote(Piece::BLACK_QUEEN, to); // TODO underpromote
					}
					else {

						// handle capturing by e. p.
						square_t target = square_t(to + 8);


						//TODO: find out which method is faster
						//if (get_piece_on(to) == 0xff) { // en passant capture
						if (move.get_move_type() == EN_PASSANT) { // en passant capture
							Square::clear_bit(pawns, target);
							Square::clear_bit(white, target);

							board[target] = 0;
						}
						else {
							// handle double step preparing the e. p.
							save_en_passant_square(move_state);
							if (to - from == -16) {
								Square::set_bit(en_passant_square, target);
								set_en_passant = true;
							}
						}
					}
					break;
				}
				case Piece::BLACK_KNIGHT:
				case Piece::BLACK_BISHOP:
				case Piece::BLACK_QUEEN:
				{

					save_en_passant_square(move_state);
					break;
				}
				case Piece::BLACK_ROOK:

					if (from == 63 && castling[2]) { // H8 //TODO
						move_state.set_cleared_kingside_castling(true);
						castling[2] = false;
					}
					else if (from == 56 && castling[3]) { //A8 // TODO
						move_state.set_cleared_queenside_castling(true);
						castling[3] = false;
					}
					save_en_passant_square(move_state);
					break;
				case Piece::BLACK_KING:


					if (from == E8 && to == C8) { //queenside castle
						Square::update_bits(black, rooks, A8, C8);
						board[A8] = 0;
						board[D8] = Piece::BLACK_ROOK;
					}
					else if (from == E8 && to == G8) { // kingside castle
						Square::update_bits(black, rooks, H8, F8);
						board[H8] = 0;
						board[F8] = Piece::BLACK_ROOK;
					}
					if (castling[2]) {
						move_state.set_cleared_kingside_castling(true);
						castling[2] = false;
					}
					if (castling[3]) {
						move_state.set_cleared_queenside_castling(true);
						castling[3] = false;
					}
					save_en_passant_square(move_state);
					break;
				}
			}
		}

	}
}