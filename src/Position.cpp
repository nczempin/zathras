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
    int tmp = row & 1;
    row = row >> 1;
    string which = tmp != 0 ? set : clear;
    retval += which;
  }
  return retval;
}

void Position::visualize_bitboard(uint_fast64_t bb, ostream& stream)
{
  //stream << "bb: " << hex << bb << dec << endl;
  stream << "  +-----------------+" << endl;
  for (int i = 7; i >= 0; --i) {
    uint_fast64_t tmp = (bb & 0xff00000000000000) >> 8 * 7; // slightly less efficient/elegant because I want the most significant byte to be on the top left
    //stream << "tmp: " << hex << tmp << dec << endl;
    //stream << "bb: " << hex << bb << dec << endl;
    bb = bb << 8;
    //stream << "bb: " << hex << bb << dec << endl;
    stream << (i + 1) << " |";
    string row_string = extract_row_string(tmp, " *");
    stream << row_string;
    stream << " |" << endl;
  }
  stream << "  +-----------------+" << endl;
  stream << "    A B C D E F G H" << endl;
}

void Position::visit_bitboard(uint_fast64_t bb, function<void(int)> f) const
{
  for (int i = 0; i < 8; ++i) {
    uint_fast64_t tmp = (bb & 0xff00000000000000) >> 8 * 7; // slightly less efficient/elegant because I want the most significant byte to be on the top left
    bb = bb << 8;

    for (int j = 0; j < 8; ++j) {
      int coord = (7 - i) * 8 + j;
      int tmp2 = tmp & 1;
      if (tmp2 != 0) {
        f(coord);
      }
      tmp = tmp >> 1;
    }
  }
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
  stream << "    A B C D E F G H" << endl;
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
//  board[48] = 1;
//  board[32] = 1;
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

vector<uint_fast64_t> Position::pregenerate_hoppers(vector<int> hops)
{
  vector<uint_fast64_t> hoppers(64);
  bitset<64> bs[64];
  for (int i = 63; i >= 0; --i) {
    for (int k : hops) {
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
    //cout << as_int << endl;
    hoppers[i] = as_int;
  }
  return hoppers;
}

vector<uint_fast64_t> Position::pregenerate_rays(int direction)
{
  vector<uint_fast64_t> rays(64);
  bitset<64> bs[64];
  for (int i = 63; i >= 0; --i) {
    for (int j = 0; j < 7; ++j) {
      int from = i + direction * j;
      int candidate = from + direction;

      if (candidate >= 0 && candidate < 64) {
        bs[i][candidate] = true;
        if (from % 8 == 7 && candidate % 8 == 0) {
          bs[i][candidate] = false;
        }
        if (from % 8 == 0 && candidate % 8 == 7) {
          bs[i][candidate] = false;
        }
      }
      if (!bs[i][candidate]) { // as soon as we hit an illegal target,
        break; // the ray ends
      }

    }
    unsigned long int as_int = bs[i].to_ulong();
    rays[i] = as_int;

  }
  return rays;
}

vector<uint_fast64_t> Position::pregenerate_knight_moves()
{
  vector<int> nm
    { 10, 17, 15, 6, -6, -15, -17, -10 };
  vector<uint_fast64_t> knight_moves = pregenerate_hoppers(nm);
  return knight_moves;
}

vector<uint_fast64_t> Position::pregenerate_king_moves()
{
  vector<int> km =
    { 1, 7, 8, 9, -1, -7, -8, -9 };
  vector<uint_fast64_t> king_moves = pregenerate_hoppers(km);
  return king_moves;
}

vector<uint_fast64_t> Position::pregenerate_bishop_moves()
{
  vector<uint_fast64_t> bishop_NE = pregenerate_rays(9);
  vector<uint_fast64_t> bishop_NW = pregenerate_rays(7);
  vector<uint_fast64_t> bishop_SE = pregenerate_rays(-9);
  vector<uint_fast64_t> bishop_SW = pregenerate_rays(-7);
  vector<uint_fast64_t> bishop_moves(64);
  for (int i = 0; i < 64; ++i) {
    bishop_moves[i] = bishop_NE[i] | bishop_NW[i] | bishop_SE[i] | bishop_SW[i];
  }
  return bishop_moves;
}

vector<uint_fast64_t> Position::pregenerate_rook_moves()
{
  vector<uint_fast64_t> rook_E = pregenerate_rays(1);
  vector<uint_fast64_t> rook_W = pregenerate_rays(-1);
  vector<uint_fast64_t> rook_S = pregenerate_rays(-8);
  vector<uint_fast64_t> rook_N = pregenerate_rays(8);
  vector<uint_fast64_t> rook_moves(64);
  for (int i = 0; i < 64; ++i) {
    rook_moves[i] = rook_E[i] | rook_W[i] | rook_S[i] | rook_N[i];
  }
  return rook_moves;

}

vector<uint_fast64_t> Position::pregenerate_queen_moves()
{
  vector<uint_fast64_t> bishop_moves = pregenerate_bishop_moves();
  vector<uint_fast64_t> rook_moves = pregenerate_rook_moves();
  vector<uint_fast64_t> queen_moves(64);
  for (int i = 0; i < 64; ++i) {
    queen_moves[i] = bishop_moves[i] | rook_moves[i];
  }
  return queen_moves;

}

void Position::place_pawn_move(int i, int a, int direction, bitset<64> bs[64])
{
  int candidate = i + a * direction;
  //    cout << i << ": " << candidate << endl;
  if (candidate >= 0 && candidate < 64) {
    bs[i][candidate] = true;
    if (i % 8 == 7 && candidate % 8 == 0) {
      bs[i][candidate] = false;
    }
    if (i % 8 == 0 && candidate % 8 == 7) {
      bs[i][candidate] = false;
    }
  }
}

vector<uint_fast64_t> Position::pregen_pawn_caps(int direction)
{
  bitset<64> bs[64];
  vector<uint_fast64_t> pawn_capture_moves(64);
  for (int i = 8; i < 56; ++i) {
    //TODO not the most elegant way to refactor/extract the function
    place_pawn_move(i, 9, direction, bs);
    place_pawn_move(i, 7, direction, bs);
  }
  for (int i = 0; i < 64; ++i) {

    unsigned long int as_int = bs[i].to_ulong();
    pawn_capture_moves[i] = as_int;
    cout << "pcm (" << i << "): " << hex << as_int << dec << endl;
  }
  return pawn_capture_moves;

}
vector<uint_fast64_t> Position::pregenerate_white_pawn_capture_moves()
{
  vector<uint_fast64_t> white_pawn_capture_moves = pregen_pawn_caps(1);
  return white_pawn_capture_moves;
}

vector<uint_fast64_t> Position::pregenerate_black_pawn_capture_moves()
{
  vector<uint_fast64_t> black_pawn_capture_moves = pregen_pawn_caps(-1);
  return black_pawn_capture_moves;

}
vector<uint_fast64_t> Position::pregen_pawn_nocaps(int start, int stop,
    int direction)
{
  bitset<64> bs[64];

  for (int i = start; i != 64 - start; i += direction) {
    int candidate = i + 8 * direction; // single step
    bs[i][candidate] = true;
  }
  vector<uint_fast64_t> pawn_no_capture_moves(64);
  for (int i = start; i != stop; i += direction) {
    int candidate = i + 16 * direction; // double step
    bs[i][candidate] = true;

    for (int i = 0; i < 64; ++i) {

      unsigned long int as_int = bs[i].to_ulong();
//    cout << "(" << i << ") " << "as_int: " << hex << as_int << dec << endl;
//    cout << "(" << 63 - i << ") " << "as_int: " << hex << as_int << dec << endl;
      pawn_no_capture_moves[i] = as_int;
    }
  }
  return pawn_no_capture_moves;

}
vector<uint_fast64_t> Position::pregenerate_white_pawn_no_capture_moves()
{
  vector<uint_fast64_t> white_pawn_no_capture_moves = pregen_pawn_nocaps(8, 16,
      1);
  return white_pawn_no_capture_moves;
}
vector<uint_fast64_t> Position::pregenerate_black_pawn_no_capture_moves()
{
  vector<uint_fast64_t> black_pawn_no_capture_moves = pregen_pawn_nocaps(56, 47,
      -1);
  return black_pawn_no_capture_moves;
}

void Position::pregenerate_moves()
{
  vector<uint_fast64_t> knight_moves = pregenerate_knight_moves();
  vector<uint_fast64_t> king_moves = pregenerate_king_moves();
  vector<uint_fast64_t> bishop_moves = pregenerate_bishop_moves();
  vector<uint_fast64_t> rook_moves = pregenerate_rook_moves();
  vector<uint_fast64_t> queen_moves = pregenerate_queen_moves();
  vector<uint_fast64_t> white_pawn_no_capture_moves =
      pregenerate_white_pawn_no_capture_moves();
  vector<uint_fast64_t> black_pawn_no_capture_moves =
      pregenerate_black_pawn_no_capture_moves();
  vector<uint_fast64_t> white_pawn_capture_moves =
      pregenerate_white_pawn_capture_moves();
  vector<uint_fast64_t> black_pawn_capture_moves =
      pregenerate_black_pawn_capture_moves();
//  for (int i = 0; i < 64; ++i) {
//    cout << i << ". bpncm: " << black_pawn_no_capture_moves[i] << endl;
//  }

//  visit_bitboard(0xffffffffffffffff, [knight_moves, &cout](int x) {
//    Position::visualize_bitboard(knight_moves[x], cout);
//  });
//  visit_bitboard(0xffffffffffffffff, [king_moves, &cout](int x) {
//    Position::visualize_bitboard(king_moves[x], cout);
//  });
//  visit_bitboard(0xffffffffffffffff, [bishop_moves, &cout](int x) {
//    Position::visualize_bitboard(bishop_moves[x], cout);
//  });
//  visit_bitboard(0xffffffffffffffff, [rook_moves, &cout](int x) {
//    Position::visualize_bitboard(rook_moves[x], cout);
//  });
//  visit_bitboard(0xffffffffffffffff, [queen_moves, &cout](int x) {
//    Position::visualize_bitboard(queen_moves[x], cout);
//  });

  //  visit_bitboard(0x00ffffffffffff00,
  //      [black_pawn_no_capture_moves, &cout](int x) {
  //        char column = 'a' + x % 8;
  //        char row = '1' + x / 8;
  //        cout << x << " = " << column << row << endl;
  //        Position::visualize_bitboard(black_pawn_no_capture_moves[x], cout);
  //      });
  visit_bitboard(0x00ffffffffffff00, [white_pawn_capture_moves, &cout](int x) {
    char column = 'a' + x % 8;
    char row = '1' + x / 8;
    cout << x << " = " << column << row << endl;
    Position::visualize_bitboard(white_pawn_capture_moves[x], cout);
  });
  visit_bitboard(0x00ffffffffffff00, [black_pawn_capture_moves, &cout](int x) {
    char column = 'a' + x % 8;
    char row = '1' + x / 8;
    cout << x << " = " << column << row << endl;
    Position::visualize_bitboard(black_pawn_capture_moves[x], cout);
  });
  //
//  visit_bitboard(0x00ffffffffffff00,
//      [white_pawn_no_capture_moves, &cout](int x) {
//        char column = 'a' + x % 8;
//        char row = '1' + x / 8;
//        cout << x << " = " << column << row << endl;
//        Position::visualize_bitboard(white_pawn_no_capture_moves[x], cout);
//      });
//  visit_bitboard(everything, [](int x) {
//    char column = 'a' + x % 8;
//    char row = '1' + x / 8;
//    cout << x << " = " << column << row << endl;
//  });

  cout << "done" << endl;
}
