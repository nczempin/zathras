/*
 * Position.cpp
 *
 *  Created on: Apr 9, 2016
 *      Author: nczempin
 */

#include "Position.h"
#include "Square.h"
#include "Piece.h"
#include <sstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

Position::Position()
{

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
void Position::set_bit(bb& b, int to)
{
  int rank = to / 8;
  int file = (to % 8);
  set_square(file, rank, b);
}
void Position::clear_bit(bb& b, int to)
{
  int rank = to / 8;
  int file = (to % 8);
  clear_square(file, rank, b);
}

bool Position::is_set_square(bb& b, int to)
{
  int t2 = (to / 8) * 8 + (7 - (to % 8));
  uint64_t ttt = 1L << (t2);
  bb aaa = b & ttt;
  return aaa != 0;
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
int Position::set_square(int file, int rank, bb& bbs)
{
  int to_twisted = 7 - file + rank * 8;
  int to = file + rank * 8;
  Position::set_square(bbs, to_twisted);
  return to;
}

int Position::clear_square(int file, int rank, bb& bbs)
{
  int to_twisted = 7 - file + rank * 8;
  int to = file + rank * 8;
  Position::clear_square(bbs, to_twisted);
  return to;
}

Position Position::create_position(const string& fen)
{
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
      cout << "right: " << right << endl;
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
  //cout << "board: " << fen_board << endl;
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
          //cout << "setting white king to " << f << ", " << r << endl;
          set_square(f, r, position.kings);
          set_square(f, r, position.white);
//          cout << "p.k: " << hex << position.kings << dec << endl;
//          cout << "p.w: " << hex << position.white << dec << endl;
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
//          cout << "rooks: " << hex << start_position.rooks << dec << endl;
//          cout << "black: " << hex << start_position.black << dec << endl;
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
//  cout << "generated position: " << endl << (position)
//      << "end of generated position" << endl;
  return position;
}

Position Position::create_start_position()
{
  const char* p = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  Position start_position = create_position(p);
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

void Position::visit_bitboard(const bb my_bb, const square_visitor f)
{
  static uint8_t lookup[] =
    { 255, 7, 6, 5, 4, 3, 2, 1, 0, //
      15, 14, 13, 12, 11, 10, 9, 8, //
      23, 22, 21, 20, 19, 18, 17, 16, //
      31, 30, 29, 28, 27, 26, 25, 24, //
      39, 38, 37, 36, 35, 34, 33, 32, //
      47, 46, 45, 44, 43, 42, 41, 40, //
      55, 54, 53, 52, 51, 50, 49, 48, //
      63, 62, 61, 60, 59, 58, 57, 56 };
  bb tmp = my_bb;
  //static bb t = 0;
  uint8_t coord = 0;
  uint8_t l = 0;
  while (true) {
    l = __builtin_ffsll(tmp);
//    cout << hex;
//    cout << "l, tmp: " << l << ", " << tmp << endl;
    if (l == 0) {
      return;
    }
    coord = lookup[l];
    //   cout << "coord: " << coord << endl;

    f(coord);
    tmp &= tmp - 1; //clear LS1B
    //   cout << "new tmp: " << tmp << endl;
    //   cout << dec;
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
  //cout << "visiting white kings" << endl;
  visit_bitboard(white & kings, [&board](int x) {
    //cout << "visiting white king at: " << x << endl;

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
  //cout << "visits for position display done" << endl;
  visualize_mailbox_board(board, stream);
  cout << "wtm: " << white_to_move << endl;
  cout << "ep: ";
  Position::visit_bitboard(en_passant_square, [](int y) {
    cout << Square::mailbox_index_to_square(y)<< endl;
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

void Position::display_all_moves(const bitboard_set& moves)
{
  visit_bitboard(0xffffffffffffffff, [moves](int x) {
    Square::print_square(x);
    Position::visualize_bitboard(moves[x], cout);
    Position::visit_bitboard(moves[x],[](int y) {
          Square::print_square(y);
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

static bool determine_colour(int piece)
{
  return piece > 0; // makes == 0 black, deal with it
}

static int8_t determine_piece(int8_t piece)
{
  if (piece > 0) {
    return piece;
  } else {
    return -piece;
  }
}
void Position::make_move(Move move)
{
  //cout << "make_move: " << move.to_string() << endl;
  uint8_t from = move.get_from();
  uint8_t to = move.get_to();

  int8_t taken = move.get_taken_piece();
  if (taken != 0) {
    bool colour = determine_colour(taken);
    if (colour) {
      clear_bit(white, to);
    } else {
      clear_bit(black, to);
    }
    int8_t piece = determine_piece(taken);
    switch (piece) {
    case 1:
      clear_bit(pawns, to);
      break;
    case 2:
      clear_bit(knights, to);
      break;
    case 3:
      clear_bit(bishops, to);
      break;
    case 4:
      clear_bit(rooks, to);
      break;
    case 5:
      clear_bit(queens, to);
      break;
    case 6:
      clear_bit(kings, to);
      break;
    default:
      cerr << "mm??" << piece << endl;
      throw piece;
    }

//    cout << "taking: " << taken << endl;
//    cout << Square::mailbox_index_to_square(from) << "-"
//        << Square::mailbox_index_to_square(to) << endl;
  }
  int8_t moving = move.get_moving_piece();
  int8_t moving_abs = moving > 0 ? moving : -moving;
  if (moving_abs > 6 || moving_abs == 0) {
    cerr << "invalid move created: moving piece: " << moving << endl;
    throw moving_abs;
  }
  //cout << "on_move: " << white_to_move << endl;
  if (white_to_move) {
    switch (moving) {
    case Piece::WHITE_PAWN:
      set_bit(pawns, to);
      set_bit(white, to);
      clear_bit(pawns, from);
      clear_bit(white, from);
      {
        // handle capturing by e. p.
        int ep_square = to - 8;
        if (is_set_square(en_passant_square, to)) { // en passant capture
          clear_bit(pawns, ep_square);
          clear_bit(black, ep_square);
        }
        // handle double step preparing the e. p.
        en_passant_square = 0x00;
        if (to - from == 16) {
          set_bit(en_passant_square, ep_square);
        }

      }
      break;
    case Piece::WHITE_KNIGHT:
      set_bit(knights, to);
      set_bit(white, to);
      clear_bit(knights, from);
      clear_bit(white, from);
      break;
    case Piece::WHITE_BISHOP:
      set_bit(bishops, to);
      set_bit(white, to);
      clear_bit(bishops, from);
      clear_bit(white, from);
      break;
    case Piece::WHITE_ROOK:
      set_bit(rooks, to);
      set_bit(white, to);
      clear_bit(rooks, from);
      clear_bit(white, from);
      break;
    case Piece::WHITE_QUEEN:
      set_bit(queens, to);
      set_bit(white, to);
      clear_bit(queens, from);
      clear_bit(white, from);
      break;
    case Piece::WHITE_KING:
      set_bit(kings, to);
      set_bit(white, to);
      clear_bit(kings, from);
      clear_bit(white, from);
      break;
    default:
      cerr << "unexpected white piece: " << moving << endl;
      cerr.flush();
      throw moving;
      break;
    }
  } else {
    switch (moving) {
    case Piece::BLACK_PAWN:
      set_bit(pawns, to);
      set_bit(black, to);
      clear_bit(pawns, from);
      clear_bit(black, from);
      {
        // handle capturing by e. p.
        int ep_square = to + 8;
        if (is_set_square(en_passant_square, to)) { // en passant capture
          clear_bit(pawns, ep_square);
          clear_bit(white, ep_square);
        }
        // handle double step preparing the e. p.
        en_passant_square = 0x00;
        if (to - from == -16) {
          set_bit(en_passant_square, ep_square);
        }

      }
      break;
    case Piece::BLACK_KNIGHT:
      set_bit(knights, to);
      set_bit(black, to);
      clear_bit(knights, from);
      clear_bit(black, from);
      break;
    case Piece::BLACK_BISHOP:
      set_bit(bishops, to);
      set_bit(black, to);
      clear_bit(bishops, from);
      clear_bit(black, from);
      break;
    case Piece::BLACK_ROOK:
      set_bit(rooks, to);
      set_bit(black, to);
      clear_bit(rooks, from);
      clear_bit(black, from);
      break;
    case Piece::BLACK_QUEEN:
      set_bit(queens, to);
      set_bit(black, to);
      clear_bit(queens, from);
      clear_bit(black, from);
      break;
    case Piece::BLACK_KING:
      set_bit(kings, to);
      set_bit(black, to);
      clear_bit(kings, from);
      clear_bit(black, from);
      break;
    default:
      double error = ((double) moving);
      cerr << "mm: unexpected black piece: " << error << " in move: "
          << move.to_string() << endl;
      throw -moving;
      break;
    }
  }

// TODO turned promoted pawn into new piece
// TODO update en passant square
// TODO update castling rights
// TODO update 3 repetitions
// TODO update 50 moves
  white_to_move = !white_to_move;
// cout << "switched on_move to " << white_to_move << endl;
}
void Position::unmake_move(Move move)
{
  // cout << "unmake_move: " << move.to_string() << endl;
  uint8_t from = move.get_from();
  uint8_t to = move.get_to();

  int8_t moving = move.get_moving_piece();
  white_to_move = !white_to_move;
  //cout << "switched on_move to " << white_to_move << endl;

  if (white_to_move) {
    switch (moving) {
    case Piece::WHITE_PAWN:
      // move pawn back
      clear_bit(pawns, to);
      clear_bit(white, to);
      set_bit(pawns, from);
      set_bit(white, from);
      {
        // handle capturing by e. p.
        int ep_square = to - 8;

        if (move.is_en_passant()) {
          //is_set_square(en_passant_square, to)) { // en passant capture
          set_bit(pawns, ep_square);
          set_bit(black, ep_square);
        }
        // handle double step preparing the e. p.
        en_passant_square = 0x00;

      }
      break;
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
      break;
    case Piece::WHITE_QUEEN:
      clear_bit(queens, to);
      clear_bit(white, to);
      set_bit(queens, from);
      set_bit(white, from);
      break;
    case Piece::WHITE_KING:
      clear_bit(kings, to);
      clear_bit(white, to);
      set_bit(kings, from);
      set_bit(white, from);
      break;
    default:
      cerr << "unexpected white piece: " << ((int) moving) << endl;
      throw moving;
      break;
    }
  } else {
    switch (moving) {
    case Piece::BLACK_PAWN:
      clear_bit(pawns, to);
      clear_bit(black, to);
      set_bit(pawns, from);
      set_bit(black, from);
      {
        // handle capturing by e. p.
        int ep_square = to + 8;

        if (move.is_en_passant()) {
          //is_set_square(en_passant_square, to)) { // en passant capture
          set_bit(pawns, ep_square);
          set_bit(white, ep_square);
        }
        // handle double step preparing the e. p.
        en_passant_square = 0x00;

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
      break;
    case Piece::BLACK_QUEEN:
      clear_bit(queens, to);
      clear_bit(black, to);
      set_bit(queens, from);
      set_bit(black, from);
      break;
    case Piece::BLACK_KING:
      clear_bit(kings, to);
      clear_bit(black, to);
      set_bit(kings, from);
      set_bit(black, from);
      break;
    default:
      double error = ((double) moving);
      cerr << "umm: unexpected black piece: " << error << endl;
      throw -moving;
      break;
    }
  }
  if (!move.is_en_passant()) {
    int8_t taken = move.get_taken_piece();
    if (taken != 0) {
      //cout << "untaken: " << taken << endl;
      bool colour = determine_colour(taken);
      if (colour) {
        set_bit(white, to);
      } else {
        set_bit(black, to);
      }
      int8_t p = determine_piece(taken);
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
      case 4:
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
    // cout << *this << endl;
  }
// TODO transform promoted piece back into pawn
// TODO update en passant square
// TODO update castling rights
// TODO update 3 repetitions
// TODO update 50 moves

}

