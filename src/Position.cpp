/*
 * Position.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#include "Position.h"
#include <bitset>

Position::Position()
{
  // TODO Auto-generated constructor stub

}

Position::~Position()
{
  // TODO Auto-generated destructor stub
}

shared_ptr<Position> Position::create_start_position()
{
  shared_ptr<Position> retval(new Position());
  return retval; //TODO this is an empty position for now
}
ostream& operator<<(ostream& stream, const Position& position)
{
  position.print(stream);
  return stream;
}

string Position::extract_row_string(uint_fast8_t row, string set)
{

  string clear = " .";
  string retval = "";

  for (int i = 0; i < 8; ++i) {
    int tmp = row & 128; // mask all but highest bit
    row = row << 1;
    string which = tmp != 0 ? set : clear;
    retval += which;
  }
  return retval;
}

void Position::visualize_bitboard(uint_fast64_t pawns1, ostream& stream)
{
  stream << "  +-----------------+" << endl;
  for (int i = 0; i < 8; ++i) {
    int tmp = pawns1 & 0xff;
    pawns1 = pawns1 >> 8;
    stream << (8 - i) << " |";
    string row_string = extract_row_string(tmp, " *");
    stream << row_string;
    stream << " |" << endl;
  }
  stream << "  +-----------------+" << endl;
}

void Position::visualize_mailbox_board(int board[64], ostream& stream)
{
  stream << "  +-----------------+" << endl;
  const char* symbols = ".PNBRQKpnbrqk*";

  for (int i = 0; i < 8; ++i) {
    cout << (8 - i) << " |";
    for (int j = 0; j < 8; ++j) {
      int coord = (7 - i) * 8 + j;
      stream << " " << symbols[board[coord]];
    }
    stream << " |" << endl;
  }
  stream << "  +-----------------+" << endl;
}

void Position::visit_bitboard(uint_fast64_t bb, function<void(int)> f) const
{
  for (int i = 0; i < 8; ++i) {
    int tmp = bb & 0xff;
    bb = bb >> 8;

    for (int j = 0; j < 8; ++j) {
      int coord = (7 - i) * 8 + j;
      int tmp2 = tmp & 128; // mask all but highest bit
      if (tmp2 != 0) {
        f(coord);
      }
      tmp = tmp << 1;
    }
  }
}

void Position::visit_mailbox_board(int board[64], void (*f)(int)) const
{
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      int coord = (7 - i) * 8 + j;
      f(board[coord]);
    }
  }
}

void Position::print(ostream& stream) const
{
  // TODO: Being a little inconsistent here with the types (int vs. uint_fastbla etc.)

  int board[64];
  for (int i = 0; i < 64; ++i) {
    board[i] = 0;
  }

  //TODO generalize (I assume with a template)
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

void Position::generate_moves()
{
  int nm[8] =
    { 10, 17, 15, 6, -6, -15, -17, -10 };
  uint_fast64_t knight_moves[64];
  bitset<64> bs[64];
  for (int i = 63; i >= 0; --i) {
    for (int k : nm) {
      int candidate = k + i;
      if (candidate >= 0 && candidate < 64) {
        bs[i][candidate] = true;
        if (i % 8 >= 6 && candidate % 8 <= 1) {
          bs[i][candidate] = false;
        }
        if (i % 8 <= 1 && candidate % 8 >= 6) {
          bs[i][candidate] = false;
        }
       }

    }
    unsigned long int as_int = bs[i].to_ulong();
    knight_moves[63 - i] = as_int;
    //    cout << as_int << endl;
  }
  int km[8] =
    { 1, 7, 8, 9, -1, -7, -8, -9 };
  uint_fast64_t king_moves[64];

  for (int i = 63; i >= 0; --i) {
//    bs[i][i] = true; // TODO only for debugging, own square shown
    //cout << i << ". " << i % 8 << endl;
    for (int k : km) {
      int candidate = k + i;
      if (candidate >= 0 && candidate < 64) {
        bs[i][candidate] = true;
        if (i % 8 == 7 && candidate % 8 == 0) {
          bs[i][candidate] = false;
        }
        if (i % 8 == 0 && candidate % 8 == 7) {
          bs[i][candidate] = false;
        }
        //cout << "king move: " << i << ": " << candidate << endl;
      }

    }
    unsigned long int as_int = bs[i].to_ulong();
    king_moves[63 - i] = as_int;
    //    cout << as_int << endl;
  }

  for (int i = 0; i < 64; ++i) {
    //  visualize_bitboard(king_moves[i], cout);
  }
  visualize_bitboard(black, cout);
  visit_bitboard(black & knights, [knight_moves, cout](int x) {
    cout << "black knight is at " << x << endl;
    Position::visualize_bitboard(knight_moves[x], cout);
  });

}
