/*
 * Position.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#include "Position.h"
#include <bitset>
#include <sstream>
#include <iomanip>

Position::Position() {
	pawns = 0x008469040000e700;
	knights = 0x4000000000000000;
	bishops = 0x0000100000100000;
	rooks = 0x0100000000000009;
	queens = 0x8000000010000000;
	kings = 0x0800000000000800;
	white = 0x800000000010ef09;
	black = 0x4984790410000000;
}

Position::~Position() {
	// TODO Auto-generated destructor stub
}

shared_ptr<Position> Position::create_start_position() {
	shared_ptr<Position> retval(new Position());
	return retval; //TODO this is an empty position for now
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
	stream << "bb: " << hex << my_bb << dec << endl;
	stream << "  +-----------------+" << endl;
	for (int i = 7; i >= 0; --i) {
		bb tmp = (my_bb & 0xff00000000000000) >> 8 * 7; // slightly less efficient/elegant because I want the most significant byte to be on the top left
		//stream << "tmp: " << hex << tmp << dec << endl;
		//stream << "bb: " << hex << bb << dec << endl;
		my_bb = my_bb << 8;
		//stream << "bb: " << hex << bb << dec << endl;
		stream << (i + 1) << " |";
		string row_string = extract_row_string(tmp, " *");
		stream << row_string;
		stream << " |" << endl;
	}
	stream << "  +-----------------+" << endl;
	stream << "    A B C D E F G H" << endl;
}

void Position::visit_bitboard(bb my_bb, function<void(int)> f) {
	for (int i = 0; i < 8; ++i) {
		bb tmp = (my_bb & 0xff00000000000000) >> 8 * 7; // slightly less efficient/elegant because I want the most significant byte to be on the top left
		my_bb = my_bb << 8;

		for (int j = 0; j < 8; ++j) {
			int coord = (7 - i) * 8 + j;
			int tmp2 = tmp & 128;
			if (tmp2 != 0) {
				f(coord);
			}
			tmp = tmp << 1;
		}
	}
}
void Position::visualize_mailbox_board(int board[64], ostream& stream) {
	stream << "  +-----------------+" << endl;
	const char* symbols = ".PNBRQKpnbrqk*";
	//ostringstream  strstr;
	for (int i = 0; i < 8; ++i) {
		stream << (8 - i) << " |";
		for (int j = 0; j < 8; ++j) {
			int coord = (7 - i) * 8 + j;
//			strstr << "coord: " << coord << ". board[coord]: " << board[coord] << endl;
			stream << " " << symbols[board[coord]];
		}
		stream << " |" << endl;
	}
	stream << "  +-----------------+" << endl;
	stream << "    A B C D E F G H" << endl;

	//stream << strstr.str() << endl;
}

void Position::visit_mailbox_board(int board[64], void (*f)(int)) {
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
	visit_bitboard(white & pawns, [&board](int x) {
		board[x] = 1;
	});
	visit_bitboard(white & knights, [&board](int x) {
		board[x] = 2;
	});
	visit_bitboard(white & bishops, [&board](int x) {
		board[x] = 3;
	});
	visit_bitboard(white & rooks, [&board](int x) {
		//cout << "WR: " << x << endl;
			board[x] = 4;
		});
	visit_bitboard(white & queens, [&board](int x) {
		board[x] = 5;
	});
	visit_bitboard(white & kings, [&board](int x) {
		board[x] = 6;
	});
	visit_bitboard(black & pawns, [&board](int x) {
		board[x] = 7;
	});
	visit_bitboard(black & knights, [&board](int x) {
		board[x] = 8;
	});
	visit_bitboard(black & bishops, [&board](int x) {
		board[x] = 9;
	});
	visit_bitboard(black & rooks, [&board](int x) {
		board[x] = 10;
	});
	visit_bitboard(black & queens, [&board](int x) {
		board[x] = 11;
	});
	visit_bitboard(black & kings, [&board](int x) {
		board[x] = 12;
	});
	visit_bitboard(~(black | white), [&board](int x) {
		board[x] = 13;
	});

	visualize_mailbox_board(board, stream);

	stream << "  +-----------------+" << endl;
	stream << "8 | Q n . . k . . r |" << endl;
	stream << "7 | p . . . . p . . |" << endl;
	stream << "6 | . p p b p . . p |" << endl;
	stream << "5 | . . . . . p . . |" << endl;
	stream << "4 | . . . q . . . . |" << endl;
	stream << "3 | . . . B . . . . |" << endl;
	stream << "2 | P P P . K P P P |" << endl;
	stream << "1 | . . . . R . . R |" << endl;
	stream << "  +-----------------+" << endl;
	stream << "    A B C D E F G H";
}

void Position::setSquare(bitset<64>& bs, int to) {
	bs[to] = true;
}

void Position::clearSquare(bitset<64>& bs, int to) {
	bs[to] = false;
}

string Position::mailboxIndexToSquare(int x) {
	//TODO separate the printing from the generating
	char column = 'a' + x % 8;
	string columnString(1, column);
	char row = '1' + x / 8;
	string rowString(1, row);
	string square = columnString + rowString;
	return square;
}

void Position::print_square(int x) { //TODO separate the printing from the generating
	string square = mailboxIndexToSquare(x);
	cout << x << " = " << square << endl;
}

void Position::display_all_moves(const bitboard_set& moves) {
	visit_bitboard(0xffffffffffffffff, [moves](int x) {
		print_square(x);
		Position::visualize_bitboard(moves[x], cout);
		Position::visit_bitboard(moves[x],[](int y) {
					Position::print_square(y);
				});
	});
}

bitboard_set Position::getPieceBitboards() {
	bitboard_set retval(8);
	//TODO figure out what to do with [0]
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
