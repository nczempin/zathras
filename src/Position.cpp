/*
 * Position.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#include "Position.h"
#include <sstream>
#include <string>
#include <sstream>
#include <vector>

Position::Position()
{
  // TODO Auto-generated constructor stub
}
//
Position::~Position()
{
  // TODO Auto-generated destructor stub
}
vector<string> &split(const string &s, char delim, vector<string> &elems)
{
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

vector<string> split(const string &s, char delim)
{
  vector<string> elems;
  split(s, delim, elems);
  return elems;
}
bool is_digit(const char c)
{
  return '0' <= c && c <= '9';
}
int Position::set_square(int file_to, int rank_to, bitset<64>& bbs)
{
  int to_twisted = 7 - file_to + rank_to * 8;
  int to = file_to + rank_to * 8;
  Position::set_square(bbs, to_twisted);
  return to;
}

int Position::clear_square(int file_to, int rank_to, bitset<64>& bbs)
{
  int to_twisted = 7 - file_to + rank_to * 8;
  int to = file_to + rank_to * 8;
  Position::clear_square(bbs, to_twisted);
  return to;
}
int Position::set_square(int file_to, int rank_to, bb& bbs)
{
  int to_twisted = 7 - file_to + rank_to * 8;
  int to = file_to + rank_to * 8;
  Position::set_square(bbs, to_twisted);
  return to;
}

int Position::clear_square(int file_to, int rank_to, bb& bbs)
{
  int to_twisted = 7 - file_to + rank_to * 8;
  int to = file_to + rank_to * 8;
  Position::clear_square(bbs, to_twisted);
  return to;
}

Position Position::create_position(const string& fen)
{
  Position start_position;
  vector<string> split_fen = split(fen, ' ');
  string fen_board = split_fen[0];
  cout << "board: " << fen_board << endl;
  vector<string> ranks = split(fen_board, '/');
  int r = 7;
  for (auto &rank : ranks) {
    int f = 0;
    for (auto &c : rank) {
//      cout << "r: " << r << endl;
//      cout << "f: " << f << endl;
//      cout << "c: " << c << endl;
      if (is_digit(c)) {
        //      cout << "digit!" << endl;
        int digit = c - '0'; //convert from ascii
        //    cout << "skip " << digit << " squares" << endl; //TODO actually do this
        f += digit;
      } else {
        //  cout << "non-digit!" << endl;
        switch (c) {
        case 'P':
          set_square(f, r, start_position.pawns);
          set_square(f, r, start_position.white);
          break;
        case 'N':
          set_square(f, r, start_position.knights);
          set_square(f, r, start_position.white);
          break;
        case 'B':
          set_square(f, r, start_position.bishops);
          set_square(f, r, start_position.white);
          break;
        case 'R':
          set_square(f, r, start_position.rooks);
          set_square(f, r, start_position.white);
          break;
        case 'Q':
          set_square(f, r, start_position.queens);
          set_square(f, r, start_position.white);
          break;
        case 'K':
          set_square(f, r, start_position.kings);
          set_square(f, r, start_position.white);
          break;
        case 'p':
          set_square(f, r, start_position.pawns);
          set_square(f, r, start_position.black);
          break;
        case 'n':
          set_square(f, r, start_position.knights);
          set_square(f, r, start_position.black);
          break;
        case 'b':
          set_square(f, r, start_position.bishops);
          set_square(f, r, start_position.black);
          break;
        case 'r':
          set_square(f, r, start_position.rooks);
          set_square(f, r, start_position.black);
//          cout << "rooks: " << hex << start_position.rooks << dec << endl;
//          cout << "black: " << hex << start_position.black << dec << endl;
          break;
        case 'q':
          set_square(f, r, start_position.queens);
          set_square(f, r, start_position.black);
          break;
        case 'k':
          set_square(f, r, start_position.kings);
          set_square(f, r, start_position.black);
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
  cout << "generated: " << endl << (start_position) << endl;
  return start_position;
}

Position Position::create_start_position()
{
  Position start_position = create_position(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  // "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -");
  return start_position;
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
    int tmp = row & 128;
    row = row << 1;
    string which = tmp != 0 ? set : clear;
    retval += which;
  }
  return retval;
}

void Position::visualize_bitboard(bb my_bb, ostream& stream)
{
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

void Position::visit_bitboard(bb my_bb, function<void(int)> f)
{
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
void Position::visualize_mailbox_board(int board[64], ostream& stream)
{
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

void Position::visit_mailbox_board(int board[64], void (*f)(int))
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

}

void Position::set_square(bitset<64>& bs, int to)
{
  bs[to] = true;
}

void Position::clear_square(bitset<64>& bs, int to)
{
  bs[to] = false;
}
void Position::set_square(bb& b, int to)
{
  bitset<64> bs(b);
  bs[to] = true;
  b = bs.to_ulong();
}

void Position::clear_square(bb& b, int to)
{
  bitset<64> bs(b);
  bs[to] = false;
  b = bs.to_ulong();

}

string Position::mailboxIndexToSquare(int x)
{
  char column = 'a' + x % 8;
  string columnString(1, column);
  char row = '1' + x / 8;
  string rowString(1, row);
  string square = columnString + rowString;
  return square;
}

void Position::print_square(int x)
{
  string square = mailboxIndexToSquare(x);
  cout << x << " = " << square << endl;

}

void Position::display_all_moves(const bitboard_set& moves)
{
  visit_bitboard(0xffffffffffffffff, [moves](int x) {
    print_square(x);
    Position::visualize_bitboard(moves[x], cout);
    Position::visit_bitboard(moves[x],[](int y) {
          Position::print_square(y);
        });
  });
}

bitboard_set Position::getPieceBitboards()
{
  bitboard_set retval;
  //TODO figure out what to do with [0]
  retval.push_back(0);
  bb p = pawns;
  bb n = knights;
  bb b = bishops;
  bb r = rooks;
  bb q = queens;
  bb k = kings;
  bb wh = white;
  bb bl = black;
  retval.push_back(p);
  retval.push_back(n);
  retval.push_back(b);
  retval.push_back(r);
  retval.push_back(q);
  retval.push_back(k);
  retval.push_back(wh);
  retval.push_back(bl);
  return retval;
}
void Position::make_move(Move move)
{
  bb from = move.get_from();
  bb to = move.get_to();
  int moving = move.get_moving_piece();
  int taken = move.get_taken_piece();
  // potentially remove captured piece from board
  // add moving piece to new position
  // remove moving piece from previous position
  // TODO turned promoted pawn into new piece
  // TODO update en passant square
  // TODO update castling rights
  // TODO update 3 repetitions
  // TODO update 50 moves
}
void Position::unmake_move(Move move)
{
  bb from = move.get_from();
  bb to = move.get_to();
  int moving = move.get_moving_piece();
  int taken = move.get_taken_piece();
  // reinstate captured piece
  // add moving piece to new position
  // remove moving piece from previous position
  // TODO transform promoted piece back into pawn
  // TODO update en passant square
  // TODO update castling rights
  // TODO update 3 repetitions
  // TODO update 50 moves
}
