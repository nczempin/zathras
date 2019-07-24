#include "Position.h"

namespace Positions {

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
	string Position::mailbox_board_simple_representation(const piece_t board[64]) {
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
	string Position::mailbox_board_debug_representation(const piece_t board[64]) {
		string retval;
		retval += "  +-----------------+\n";
		const char* symbols = ".PNBRQKpnbrqk*";
		for (int i = 0; i < 8; ++i) {
			retval.append(to_string(8 - i));
			retval.append(" |");
			for (int j = 0; j < 8; ++j) {
				int coord = (7 - i) * 8 + j;
				int val = int(board[coord]);
				if (0 <= val && val <= 9) { // TODO use proper string formatting
					retval.append(" ");
				}
				retval.append(to_string(val));
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
		retval += mailbox_board_debug_representation(board);
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
		retval += mailbox_board_simple_representation(mboard);
		retval += mailbox_board_debug_representation(board);
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

	void Position::debug_position() {
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

}