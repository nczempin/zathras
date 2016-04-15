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
	// TODO Auto-generated constructor stub

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
	//stream << "bb: " << hex << bb << dec << endl;
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

pair<bitboard_set, bitboard_set> Position::pregenerate_hoppers(
		vector<int> hops) {
	bitboard_set hoppers_attacking(64);
	bitset<64> attacking[64];
	for (int i = 0; i < 64; ++i) {
		for (int k : hops) {
			int candidate = k + i;
			if (candidate >= 0 && candidate < 64) {
				int file_from = i % 8;
				int file_to = candidate % 8;
				int rank_to = candidate / 8;
				int to = 7 - file_to + rank_to * 8;
				setSquare(attacking[i], to);
				if (file_from >= 6 && file_to <= 1) {
					clearSquare(attacking[i], to);
				}
				if (file_from <= 1 && file_to >= 6) {
					clearSquare(attacking[i], to);
				}
			}
		}
	}
	// collect them all
	for (int i = 0; i < 64; ++i) {
		hoppers_attacking[i] = attacking[i].to_ulong();
	}
	pair<bitboard_set, bitboard_set> p(hoppers_attacking, hoppers_attacking);
	return p;
}
pair<bitboard_set, bitboard_set> Position::pregenerate_rays(int direction) {
	bitboard_set rays(64);
	bitset<64> bs[64];
	for (int i = 63; i >= 0; --i) {
		for (int j = 0; j < 7; ++j) {
			int from = i + direction * j;
			int candidate = from + direction;
			int file_to = candidate % 8;
			int rank_to = candidate / 8;
			int to = 7 - file_to + rank_to * 8;

			if (candidate >= 0 && candidate < 64) {
				setSquare(bs[i], to);
				if (from % 8 == 7 && candidate % 8 == 0) {
					clearSquare(bs[i], to);
				}
				if (from % 8 == 0 && candidate % 8 == 7) {
					clearSquare(bs[i], to);
				}
			}
			if (!bs[i][to]) { // as soon as we hit an illegal target,
				break; // the ray ends
			}
		}
		unsigned long int as_int = bs[i].to_ulong();
		rays[i] = as_int;
	}
	pair<bitboard_set, bitboard_set> p(rays, rays);
	return p;
}

pair<bitboard_set, bitboard_set> Position::pregenerate_knight_moves() {
	vector<int> nm { 10, 17, 15, 6, -6, -15, -17, -10 };
	bitboard_set knight_moves = pregenerate_hoppers(nm).first;
	pair<bitboard_set, bitboard_set> p(knight_moves, knight_moves);
	return p;
}

pair<bitboard_set, bitboard_set> Position::pregenerate_king_moves() {
	vector<int> km = { 1, 7, 8, 9, -1, -7, -8, -9 };
	bitboard_set king_moves = pregenerate_hoppers(km).first;
	pair<bitboard_set, bitboard_set> p(king_moves, king_moves);
	return p;
}

pair<bitboard_set, bitboard_set> Position::pregenerate_bishop_moves() {
	pair<bitboard_set, bitboard_set> bishop_NE = pregenerate_rays(9);
	pair<bitboard_set, bitboard_set> bishop_NW = pregenerate_rays(7);
	pair<bitboard_set, bitboard_set> bishop_SE = pregenerate_rays(-9);
	pair<bitboard_set, bitboard_set> bishop_SW = pregenerate_rays(-7);
	bitboard_set bishop_moves(64);
	for (int i = 0; i < 64; ++i) {
		bishop_moves[i] = bishop_NE.first[i] | bishop_NW.first[i]
				| bishop_SE.first[i] | bishop_SW.first[i];
	}
	pair<bitboard_set, bitboard_set> p(bishop_moves, bishop_moves);
	return p;
}

pair<bitboard_set, bitboard_set> Position::pregenerate_rook_moves() {
	pair<bitboard_set, bitboard_set> rook_E = pregenerate_rays(1);
	pair<bitboard_set, bitboard_set> rook_W = pregenerate_rays(-1);
	pair<bitboard_set, bitboard_set> rook_S = pregenerate_rays(-8);
	pair<bitboard_set, bitboard_set> rook_N = pregenerate_rays(8);
	bitboard_set rook_moves(64);
	for (int i = 0; i < 64; ++i) {
		rook_moves[i] = rook_E.first[i] | rook_W.first[i] | rook_S.first[i]
				| rook_N.first[i];
	}
	pair<bitboard_set, bitboard_set> p(rook_moves, rook_moves);
	return p;
}

pair<bitboard_set, bitboard_set> Position::pregenerate_queen_moves() {
	pair<bitboard_set, bitboard_set> bishop_moves = pregenerate_bishop_moves();
	pair<bitboard_set, bitboard_set> rook_moves = pregenerate_rook_moves();
	bitboard_set queen_moves(64);
	for (int i = 0; i < 64; ++i) {
		queen_moves[i] = bishop_moves.first[i] | rook_moves.first[i];
	}
	pair<bitboard_set, bitboard_set> p(queen_moves, queen_moves);
	return p;
}

void Position::place_pawn_move(int from, int steps, int direction,
		bitset<64> bs[64]) {
	int candidate = from + steps * direction;
	int file_to = candidate % 8;
	int rank_to = candidate / 8;
	int to = 7 - file_to + rank_to * 8;

	if (candidate >= 0 && candidate < 64) {
		setSquare(bs[from], to);
		if (from % 8 == 7 && candidate % 8 == 0) {
			clearSquare(bs[from], to);
		}
		if (from % 8 == 0 && candidate % 8 == 7) {
			clearSquare(bs[from], to);
		}
	}
}

pair<bitboard_set, bitboard_set> Position::pregen_pawn_caps(int direction) {
	bitset<64> bs[64];
	bitboard_set pawn_capture_moves(64);
	for (int i = 8; i < 56; ++i) {
		//TODO not the most elegant way to refactor/extract the function
		place_pawn_move(i, 9, direction, bs);
		place_pawn_move(i, 7, direction, bs);
	}
	for (int i = 0; i < 64; ++i) {
		unsigned long int as_int = bs[i].to_ulong();
		pawn_capture_moves[i] = as_int;
	}
	pair<bitboard_set, bitboard_set> p(pawn_capture_moves, pawn_capture_moves);
	return p;
}
pair<bitboard_set, bitboard_set> Position::pregenerate_white_pawn_capture_moves() {
	pair<bitboard_set, bitboard_set> white_pawn_capture_moves =
			pregen_pawn_caps(1);
	return white_pawn_capture_moves;
}

pair<bitboard_set, bitboard_set> Position::pregenerate_black_pawn_capture_moves() {
	pair<bitboard_set, bitboard_set> black_pawn_capture_moves =
			pregen_pawn_caps(-1);
	return black_pawn_capture_moves;

}
bitboard_set Position::pregen_pawn_nocaps(int start, int stop, int direction) {
	bitset<64> bs[64];

	for (int i = start; i != 64 - start; i += direction) {
		int candidate = i + 8 * direction; // single step
		int file_to = candidate % 8;
		int rank_to = candidate / 8;
		int to = 7 - file_to + rank_to * 8;
		setSquare(bs[i], to);
	}
	bitboard_set pawn_no_capture_moves(64);
	for (int i = start; i != stop; i += direction) {
		int candidate = i + 16 * direction; // double step
		int file_to = candidate % 8;
		int rank_to = candidate / 8;
		int to = 7 - file_to + rank_to * 8;
		setSquare(bs[i], to);
	}
	for (int i = 0; i < 64; ++i) {
		unsigned long int as_int = bs[i].to_ulong();
		pawn_no_capture_moves[i] = as_int;
	}
	return pawn_no_capture_moves;

}
bitboard_set Position::pregenerate_white_pawn_no_capture_moves() {
	bitboard_set white_pawn_no_capture_moves = pregen_pawn_nocaps(8, 16, 1);
	return white_pawn_no_capture_moves;
}
bitboard_set Position::pregenerate_black_pawn_no_capture_moves() {
	bitboard_set black_pawn_no_capture_moves = pregen_pawn_nocaps(56, 47, -1);
	return black_pawn_no_capture_moves;
}

void Position::print_square(int x) { //TODO separate the printing from the generating
	char column = 'a' + x % 8;
	char row = '1' + x / 8;
	cout << x << " = " << column << row << endl;
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

void Position::pregenerate_moves() {
	pair<bitboard_set, bitboard_set> knight_moves = pregenerate_knight_moves();
	pair<bitboard_set, bitboard_set> king_moves = pregenerate_king_moves();
	pair<bitboard_set, bitboard_set> bishop_moves = pregenerate_bishop_moves();
	pair<bitboard_set, bitboard_set> rook_moves = pregenerate_rook_moves();
	pair<bitboard_set, bitboard_set> queen_moves = pregenerate_queen_moves();
	bitboard_set white_pawn_no_capture_moves =
			pregenerate_white_pawn_no_capture_moves();
	bitboard_set black_pawn_no_capture_moves =
			pregenerate_black_pawn_no_capture_moves();
	pair<bitboard_set, bitboard_set> white_pawn_capture_moves =
			pregenerate_white_pawn_capture_moves();
	pair<bitboard_set, bitboard_set> black_pawn_capture_moves =
			pregenerate_black_pawn_capture_moves();
	display_all_moves(black_pawn_capture_moves.first);
}
