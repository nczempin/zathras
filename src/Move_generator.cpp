/*
 * Move_generator.cpp
 *
 *  Created on: 16.04.2016
 *      Author: nczempin
 */

#include "Move_generator.h"

#include <bitset>
#include <utility>
#include <vector>
#include "Position.h"
#include "Square.h"
#include "Piece.h"

Move_generator::Move_generator()
{
}

Move_generator::~Move_generator()
{
  // TODO Auto-generated destructor stub
}

bitboard_set Move_generator::knight_moves = pregenerate_knight_moves();

bitboard_set Move_generator::king_moves = pregenerate_king_moves();
bitboard_set Move_generator::bishop_moves = pregenerate_bishop_moves();
bitboard_set Move_generator::rook_moves = pregenerate_rook_moves();
bitboard_set Move_generator::queen_moves = pregenerate_queen_moves();
bitboard_set Move_generator::white_pawn_no_capture_moves =
    pregenerate_white_pawn_no_capture_moves();
bitboard_set Move_generator::black_pawn_no_capture_moves =
    pregenerate_black_pawn_no_capture_moves();
bitboard_set Move_generator::white_pawn_capture_moves =
    pregenerate_white_pawn_capture_moves();
bitboard_set Move_generator::black_pawn_capture_moves =
    pregenerate_black_pawn_capture_moves();

//TODO move these outside
int Move_generator::set_square(int file_to, int rank_to, bitset<64>& bbs)
{
  int to_twisted = 7 - file_to + rank_to * 8;
  int to = file_to + rank_to * 8;
  Position::set_square(bbs, to_twisted);
  return to;
}

int Move_generator::clear_square(int file_to, int rank_to, bitset<64>& bbs)
{
  int to_twisted = 7 - file_to + rank_to * 8;
  int to = file_to + rank_to * 8;
  Position::clear_square(bbs, to_twisted);
  return to;
}

bitboard_set Move_generator::pregenerate_rays(int8_t direction)
{
  bitboard_set rays(64);
  bitset<64> bs[64];
  for (int i = 63; i >= 0; --i) {
    bitset<64> bbs = bs[i];
    for (int j = 0; j < 7; ++j) {
      int from = i + direction * j;
      int candidate = from + direction;
      int file_to = candidate % 8;
      int rank_to = candidate / 8;
      const int to = 7 - file_to + rank_to * 8;

      if (candidate >= 0 && candidate < 64) {
        set_square(file_to, rank_to, bbs);
        if (from % 8 == 7 && candidate % 8 == 0) {
          clear_square(file_to, rank_to, bbs);
        }
        if (from % 8 == 0 && candidate % 8 == 7) {
          clear_square(file_to, rank_to, bbs);
        }
      }
      if (!bbs[to]) { // as soon as we hit an illegal target,
        break; // the ray ends
      }
    }

    unsigned long int as_int = bbs.to_ulong();
    rays[i] = as_int;
  }
  return rays;
}

bitboard_set Move_generator::pregenerate_hoppers(vector<int> hops)
{
  bitboard_set hoppers_attacking(64);
  bitset<64> attacking[64];
  for (int i = 0; i < 64; ++i) {
    for (int k : hops) {
      int candidate = k + i;
      if (candidate >= 0 && candidate < 64) {
        int file_from = i % 8;
        int file_to = candidate % 8;
        int rank_to = candidate / 8;
        set_square(file_to, rank_to, attacking[i]);
        if (file_from >= 6 && file_to <= 1) {
          clear_square(file_to, rank_to, attacking[i]);
        }
        if (file_from <= 1 && file_to >= 6) {
          clear_square(file_to, rank_to, attacking[i]);
        }
      }
    }
  }
  // collect them all
  for (int i = 0; i < 64; ++i) {
    hoppers_attacking[i] = attacking[i].to_ulong();
  }
  return hoppers_attacking;
}
bitboard_set Move_generator::pregenerate_knight_moves()
{
  vector<int> nm
    { 10, 17, 15, 6, -6, -15, -17, -10 };
  bitboard_set knight_moves = pregenerate_hoppers(nm);
  return knight_moves;
}

bitboard_set Move_generator::pregenerate_king_moves()
{
  vector<int> km =
    { 1, 7, 8, 9, -1, -7, -8, -9 };
  bitboard_set king_moves = pregenerate_hoppers(km);
  return king_moves;
}

bitboard_set Move_generator::pregenerate_bishop_moves()
{
  bitboard_set bishop_NE = pregenerate_rays(9);
  bitboard_set bishop_NW = pregenerate_rays(7);
  bitboard_set bishop_SE = pregenerate_rays(-9);
  bitboard_set bishop_SW = pregenerate_rays(-7);
  bitboard_set bishop_moves(64);
  for (int i = 0; i < 64; ++i) {
    bishop_moves[i] = bishop_NE[i] | bishop_NW[i] | bishop_SE[i] | bishop_SW[i];
  }
  return bishop_moves;
}

bitboard_set Move_generator::pregenerate_rook_moves()
{
  bitboard_set rook_E = pregenerate_rays(1);
  bitboard_set rook_W = pregenerate_rays(-1);
  bitboard_set rook_S = pregenerate_rays(-8);
  bitboard_set rook_N = pregenerate_rays(8);
  bitboard_set rook_moves(64);
  for (int i = 0; i < 64; ++i) {
    rook_moves[i] = rook_E[i] | rook_W[i] | rook_S[i] | rook_N[i];
  }
  return rook_moves;
}

bitboard_set Move_generator::pregenerate_queen_moves()
{
  bitboard_set bishop_moves = pregenerate_bishop_moves();
  bitboard_set rook_moves = pregenerate_rook_moves();
  bitboard_set queen_moves(64);
  for (int i = 0; i < 64; ++i) {
    queen_moves[i] = bishop_moves[i] | rook_moves[i];
  }
  return queen_moves;
}

void Move_generator::pregenerate_moves()
{
}

bitboard_set Move_generator::pregen_pawn_caps(int direction)
{
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
  return pawn_capture_moves;
}
bitboard_set Move_generator::pregenerate_white_pawn_capture_moves()
{
  bitboard_set white_pawn_capture_moves = pregen_pawn_caps(Piece::WHITE_PAWN);
  return white_pawn_capture_moves;
}

bitboard_set Move_generator::pregenerate_black_pawn_capture_moves()
{
  bitboard_set black_pawn_capture_moves = pregen_pawn_caps(Piece::BLACK_PAWN);
  return black_pawn_capture_moves;

}
void Move_generator::place_pawn_move(int from, int steps, int direction,
    bitset<64> bs[64])
{
  int candidate = from + steps * direction;
  int file_to = candidate % 8;
  int rank_to = candidate / 8;
//	int to = 7 - file_to + rank_to * 8;

  if (candidate >= 0 && candidate < 64) {
    set_square(file_to, rank_to, bs[from]);
    if (from % 8 == 7 && candidate % 8 == 0) {
      clear_square(file_to, rank_to, bs[from]);
    }
    if (from % 8 == 0 && candidate % 8 == 7) {
      clear_square(file_to, rank_to, bs[from]);
    }
  }
}

bitboard_set Move_generator::pregen_pawn_nocaps(int start, int stop,
    int8_t direction)
{
  bitset<64> bs[64];

  for (int i = start; i != 64 - start; i += direction) {
    int candidate = i + 8 * direction; // single step
    int file_to = candidate % 8;
    int rank_to = candidate / 8;
    set_square(file_to, rank_to, bs[i]);
  }
  bitboard_set pawn_no_capture_moves(64);
  for (int i = start; i != stop; i += direction) {
    int candidate = i + 16 * direction; // double step
    int file_to = candidate % 8;
    int rank_to = candidate / 8;
    set_square(file_to, rank_to, bs[i]);
  }
  for (int i = 0; i < 64; ++i) {
    unsigned long int as_int = bs[i].to_ulong();
    pawn_no_capture_moves[i] = as_int;
  }
  return pawn_no_capture_moves;

}
bitboard_set Move_generator::pregenerate_white_pawn_no_capture_moves()
{
  bitboard_set white_pawn_no_capture_moves = pregen_pawn_nocaps(8, 16,
      Piece::WHITE_PAWN);
  return white_pawn_no_capture_moves;
}
bitboard_set Move_generator::pregenerate_black_pawn_no_capture_moves()
{
  bitboard_set black_pawn_no_capture_moves = pregen_pawn_nocaps(56, 47,
      Piece::BLACK_PAWN);
  return black_pawn_no_capture_moves;
}

void Move_generator::print_moves_raw(const bb sub_position,
    const bitboard_set all_moves, const Position position)
{
  visit_moves_raw(sub_position, all_moves, [](int pc, int x, int y, int cpt) {
    string from = Square::mailbox_index_to_square(x);
    string to = Square::mailbox_index_to_square(y);
    cout << from << to << endl;
  }, position.is_white_to_move());
}

int Move_generator::find_captured_piece(int y)
{
  //cout << "looking for captured piece at: " << y << endl;
  int captured = 0;
  for (int i = 1; i < 7; ++i) {
    if (Position::is_set_square(pieces[i], y)) {
      captured = i;
      if (Position::is_set_square(pieces[8], y)) { //black
        captured = -i;
      }
      break;
    }
  }
  if (p->en_passant_square != 0) {
    Position::visit_bitboard(p->en_passant_square, [&captured, y](int square) {
      if (y == square) {
        captured = 1; //
        if (square > 31) {
          captured = -1;
        }
      }
    });
  }
//cout << "found: " << captured << endl;
  return captured;
}

void Move_generator::visit_capture_moves(const bb sub_position,
    const bitboard_set all_moves, const move_visitor f, const bb other_colour,
    const int8_t moving)
{
//cout << "visit capture moves" << endl;
  Position::visit_bitboard(sub_position,
      [this, all_moves, f, other_colour, moving](uint8_t from) {
        bb raw_moves = all_moves[from];
        bb moves = raw_moves & (other_colour);
        if (p->en_passant_square != 0x00 && (moving == Piece::WHITE_PAWN || moving == Piece::BLACK_PAWN)) {
          moves = raw_moves & (other_colour | p->en_passant_square);
        }

        Position::visit_bitboard(moves, [this, from, f, moving](uint8_t to) {
              if (p->white_to_move && moving <= 0) {
                cerr << "wrong mover: " << moving << endl;
                throw from;
              }
              if (!p->white_to_move && moving >= 0) {
                cerr << "wrong mover: "<< moving << endl;
                throw from;
              }

              int8_t captured = find_captured_piece(to);

              f(moving, from, to, captured);
            }
        );
      });
}
void Move_generator::visit_non_capture_moves(const bb sub_position,
    const bitboard_set all_moves, move_visitor f, bb other_colour,
    int8_t moving)
{
  Position::visit_bitboard(sub_position,
      [all_moves, f, other_colour, moving](uint8_t from) {
        bb raw_moves = all_moves[from];
        bb moves = raw_moves & ~other_colour;
        Position::visit_bitboard(moves, [from, f, moving](uint8_t to) {
              f(moving, from, to, 0);
            }
        );
      });
}
static bool on_same_file(int x, int y)
{
  return (x % 8) == (y % 8);
}
static bool on_same_rank(int x, int y)
{
  return (x / 8) == (y / 8);
}
static int determine_diagonal(int x, int y)
{
  int smaller = x;
  int larger = y;
  if (y < x) {
    smaller = y;
    larger = x;
  }
  int diff = larger - smaller;
  if (diff % 9 == 0) {
    return 1;
  } else if (diff % 7 == 0) {
    return -1;
  }
  return 0;
}

bool Move_generator::is_anything_between(int x, int y, bb occupied)
{
  if (x == y) {
    return false;
  }
//TODO this can be done much more elegantly and much more efficiently
  int smaller = x;
  int larger = y;
  if (y < x) {
    smaller = y;
    larger = x;
  }
  int rank = smaller / 8;
  int file = smaller % 8;
  int diff = larger - smaller;
  if (on_same_file(x, y)) {
    if (diff == 8) {
      return false;
    }
// evil magic numbers
    bitset<64> between = 0;
    for (int i = 1; i < diff / 8; ++i) {
      int rank_to = rank + i;
      set_square(file, rank_to, between);
      int to = file + rank_to * 8;
      //cout << "setting " << Position::mailboxIndexToSquare(to) << endl;
    }
    bb intersection = between.to_ulong() & occupied;
    return intersection != 0;
  } else if (on_same_rank(x, y)) {
//again with the elegance
    if (diff == 1) {
      //cout << "diff = 1" << endl;
      return false;
    }
// evil magic numbers
    bitset<64> between = 0;
    for (int i = 1; i < diff; ++i) {

      int to = set_square(file + i, rank, between);
    }
    bb intersection = between.to_ulong() & occupied;
    return intersection != 0;
  }
  int diagonal = determine_diagonal(x, y);
  if (diagonal == 0) {
    cerr << "sliding move gen called with non-sliding move" << endl;
    throw 439; //
  }
  if (diff == 8 + diagonal) {
    return false; // adjacent
  }
  bitset<64> between = 0;
  int rank_larger = larger / 8;
  int i = 1;
  while (rank + i < rank_larger) {
    if (diagonal == -1 && file + i * diagonal % 8 == 7) {
      break;
    }
    if (diagonal == 1 && rank + i * diagonal % 8 == 0) {
      break;
    }
    int to = set_square(file + i * diagonal, rank + i, between);
    ++i;
  }
  bb intersection = between.to_ulong() & occupied;
  return intersection != 0;
}

void Move_generator::visit_non_capture_ray_moves(const bb sub_position,
    const bitboard_set all_moves, move_visitor f, bb occupied, int moving)
{
  Position::visit_bitboard(sub_position,
      [all_moves, f, occupied, moving](int x) {
        bb raw_moves = all_moves[x];
        bb moves = raw_moves & ~occupied;
        Position::visit_bitboard(moves, [x, f, occupied, moving](int y) {
              bool b = is_anything_between(x, y, occupied);
              if (!b) {
                f(moving, x, y, 0);
              }
            }
        );
      });
}
void Move_generator::visit_capture_ray_moves(const bb sub_position,
    const bitboard_set all_moves, move_visitor f, bb occupied, bb other_colour,
    int moving)
{
  Position::visit_bitboard(sub_position,
      [this, all_moves, f, occupied, other_colour, moving](int x) {
        bb raw_moves = all_moves[x];
        bb moves = raw_moves & other_colour;
        Position::visit_bitboard(moves, [this, x, f, occupied, moving](int y) {
              bool b = is_anything_between(x, y, occupied);
              if (!b) {
                int captured = find_captured_piece(y);
                f(moving, x, y, captured);
              }
            }
        );
      });
}

void Move_generator::visit_moves_raw(const bb sub_position,
    const bitboard_set all_moves, move_visitor f, int moving)
{

  Position::visit_bitboard(sub_position, [all_moves, f, moving](int x) {
    Position::visit_bitboard(all_moves[x], [x, f, moving](int y) {
          int captured = -98;
          f(moving, x, y, captured); //TODO get the captured piece from somewhere
    }
);
});
}

bb Move_generator::filter_occupied_squares(bool white_to_move, bb occupied,
    const bitboard_set& all_moves, int x)
{
//TODO simplify the distinction between white's move and black's move
  const bb jump_over = white_to_move ? Position::BB_RANK3 : Position::BB_RANK6;
  bb jump_over_occupied = jump_over & occupied;
  bb jump_over_occupants_shifted =
      white_to_move ? jump_over_occupied << 8 : jump_over_occupied >> 8;
  bb all_moves_from_here = all_moves[x];
  const bb jump_onto = white_to_move ? Position::BB_RANK4 : Position::BB_RANK5;
  bb all_moves_to_fifth =
      (x >= 48 && x < 56) ? all_moves_from_here & jump_onto : 0;
  bb all_moves_to_fourth =
      (x >= 8 && x < 16) ? all_moves_from_here & jump_onto : 0;
  bb all_moves_to_jump =
      white_to_move ? all_moves_to_fourth : all_moves_to_fifth;
  bb filter_out = all_moves_to_jump & jump_over_occupants_shifted;
  bb moves = (all_moves_from_here & ~occupied) & ~filter_out;
  return moves;
}

void Move_generator::visit_pawn_nocaps(const bb sub_position,
    const bitboard_set all_moves, move_visitor f, bb occupied, int8_t moving,
    bool white_to_move)
{
  Position::visit_bitboard(sub_position,
      [all_moves, f, occupied, moving, white_to_move](int x) {
        bb moves = filter_occupied_squares(white_to_move, occupied, all_moves, x);
        Position::visit_bitboard(moves, [x, f, moving](int y) {
              f(moving, x, y, 0);
            }
        );
      });
}

vector<Move> Move_generator::generate_capture_moves()
{
  pieces = p->getPieceBitboards();
  vector<Move> moves;
  moves.clear();
  function<void(int8_t, uint8_t, uint8_t, int8_t)> f =
      [&moves, this](int8_t moving, uint8_t from, uint8_t to, int8_t captured) {
        bool en_passant = false;
        if ((moving == Piece::WHITE_PAWN || moving == Piece::BLACK_PAWN) &&Position::is_set_square(p->en_passant_square, to)) {
          en_passant = true;
        }
        Move m (moving, from, to, captured, en_passant);
        moves.push_back(m);
      };

//TODO generalize, obviously
  bb white_pawns = pieces[Piece::PAWN] & pieces[Piece::WHITE];
  bb white_knights = pieces[Piece::KNIGHT] & pieces[Piece::WHITE];
  bb white_bishops = pieces[Piece::BISHOP] & pieces[Piece::WHITE];
  bb white_rooks = pieces[Piece::ROOK] & pieces[Piece::WHITE];
  bb white_queens = pieces[Piece::QUEEN] & pieces[Piece::WHITE];
  bb white_kings = pieces[Piece::KING] & pieces[Piece::WHITE];
  bb black_pawns = pieces[Piece::PAWN] & pieces[Piece::BLACK];
  bb black_knights = pieces[Piece::KNIGHT] & pieces[Piece::BLACK];
  bb black_bishops = pieces[Piece::BISHOP] & pieces[Piece::BLACK];
  bb black_rooks = pieces[Piece::ROOK] & pieces[Piece::BLACK];
  bb black_queens = pieces[Piece::QUEEN] & pieces[Piece::BLACK];
  bb black_kings = pieces[Piece::KING] & pieces[Piece::BLACK];
///

  if (p->white_to_move) {
    // cout << "capturing with white pawns" << endl;
    visit_capture_moves(white_pawns, white_pawn_capture_moves, f,
        pieces[Piece::BLACK], Piece::WHITE_PAWN);
    visit_capture_moves(white_knights, knight_moves, f, pieces[Piece::BLACK],
        Piece::WHITE_KNIGHT);
    visit_capture_moves(white_kings, king_moves, f, pieces[Piece::BLACK],
        Piece::WHITE_KING);
    visit_capture_ray_moves(white_queens, rook_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_QUEEN);
    visit_capture_ray_moves(white_rooks, rook_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_ROOK);
    visit_capture_ray_moves(white_bishops, bishop_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_BISHOP);
    visit_capture_ray_moves(white_queens, bishop_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_QUEEN);
  } else {
    visit_capture_moves(black_pawns, black_pawn_capture_moves, f,
        pieces[Piece::WHITE], Piece::BLACK_PAWN);
    visit_capture_moves(black_knights, knight_moves, f, pieces[Piece::WHITE],
        Piece::BLACK_KNIGHT);
    visit_capture_moves(black_kings, king_moves, f, pieces[Piece::WHITE],
        Piece::BLACK_KING);
    visit_capture_ray_moves(black_queens, rook_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_QUEEN);
    visit_capture_ray_moves(black_rooks, rook_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_ROOK);
    visit_capture_ray_moves(black_bishops, bishop_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_BISHOP);
    visit_capture_ray_moves(black_queens, bishop_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_QUEEN);

  }
  return moves;
}

void Move_generator::attempt_castle(const move_visitor f, const int8_t piece,
    const uint8_t king_square, const int8_t direction)
{
  //1. check if squares between king and rook are free.
  uint8_t next_square = king_square + direction;
  uint8_t target_square = king_square + direction * 2;

  if (p->is_set_square(p->white | p->black, next_square)) {
    return;
  }
  if (p->is_set_square(p->white | p->black, target_square)) {
    return;
  }
  if (direction == -1) { // queen side
    if (p->is_set_square(p->white | p->black, target_square - 1)) { // b1/b8
      return;
    }

  }

  //2. check if squares between king and rook are attacked.
  p->white_to_move = !p->white_to_move;

//  cout << Square::mailbox_index_to_square(king_square) << "->"
//      << (int) direction << endl;
  bool attacked = is_attacked(king_square);
  //cout << attacked << "." << next << "." << target << endl;
  if (attacked) {
    p->white_to_move = !p->white_to_move;
    //cout << " is check" << endl;
    return;
  }
  bool next = is_attacked(next_square);
  if (next) {
    //cout << "next is attacked" << endl;
    p->white_to_move = !p->white_to_move;
    return;
  }
  bool target = is_attacked(target_square);
  if (target) {
    //cout << "target is attacked" << endl;
    p->white_to_move = !p->white_to_move;
    return;
  }
  p->white_to_move = !p->white_to_move;

  f(piece, king_square, king_square + direction * 2, 0);
}

void Move_generator::generate_castling(const move_visitor& f,
    bool white_to_move)
{
  int piece = Piece::WHITE_KING;
  uint8_t king_square = Square::E1;
  if (!white_to_move) {
    piece = Piece::BLACK_KING;
    king_square = Square::E8;
  }
  //castling:
  static int8_t king_jump_direction[] =
    { 1, -1, 1, -1 };
  static bool colour[] =
    { true, true, false, false };
  for (int i = 0; i < 4; ++i) {
    //cout << p->castling[i];
    if (white_to_move == colour[i] && p->castling[i]) {
      attempt_castle(f, piece, king_square, king_jump_direction[i]);
    }
  }
  //cout << endl;
}

Move_container Move_generator::generate_moves(shared_ptr<Position> position,
    size_t depth)
{
  p = position;
//  cout << p->white_to_move << endl;
  pieces = p->getPieceBitboards();
//  cout << "piece bitboards: " << endl;
//  for (auto &piece : pieces) {
//    cout << piece << endl;
//  }
//  size_t bunch = 35;
//  size_t next_max = bunch;
//  int i = 0;

  Move_container moves = Move_container::get(depth);
//moves.reserve(35);
  moves.reset();
// cout << "should be 0: " << moves.size() << endl;
  move_visitor f =
      [&moves, this](int8_t moving, uint8_t from, uint8_t to, int8_t captured) {
        bool en_passant = false;
        if ((moving == Piece::WHITE_PAWN || moving == Piece::BLACK_PAWN) &&Position::is_set_square(p->en_passant_square, to)) {
          en_passant = true;
        }
        moves.add_move(moving, from, to, captured, en_passant);
      };

//TODO generalize, obviously
  bb white_pawns = pieces[Piece::PAWN] & pieces[Piece::WHITE];
  bb white_knights = pieces[Piece::KNIGHT] & pieces[Piece::WHITE];
  bb white_bishops = pieces[Piece::BISHOP] & pieces[Piece::WHITE];
  bb white_rooks = pieces[Piece::ROOK] & pieces[Piece::WHITE];
  bb white_queens = pieces[Piece::QUEEN] & pieces[Piece::WHITE];
  bb white_kings = p->kings & p->white;
//  cout << hex << endl;
//  cout << "k: " << p->kings << endl;
//  cout << "w: " << p->white << endl;
//  cout << "wk: " << white_kings << endl;
//  cout << dec << endl;
  bb black_pawns = pieces[Piece::PAWN] & pieces[Piece::BLACK];
  bb black_knights = pieces[Piece::KNIGHT] & pieces[Piece::BLACK];
  bb black_bishops = pieces[Piece::BISHOP] & pieces[Piece::BLACK];
  bb black_rooks = pieces[Piece::ROOK] & pieces[Piece::BLACK];
  bb black_queens = pieces[Piece::QUEEN] & pieces[Piece::BLACK];
  bb black_kings = pieces[Piece::KING] & pieces[Piece::BLACK];
///

  if (p->white_to_move) {
    visit_capture_moves(white_pawns, white_pawn_capture_moves, f,
        pieces[Piece::BLACK], Piece::WHITE_PAWN);
    visit_pawn_nocaps(white_pawns, white_pawn_no_capture_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_PAWN, true);
    visit_capture_moves(white_knights, knight_moves, f, pieces[Piece::BLACK],
        Piece::WHITE_KNIGHT);
    visit_non_capture_moves(white_knights, knight_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_KNIGHT);
    visit_capture_moves(white_kings, king_moves, f, pieces[Piece::BLACK],
        Piece::WHITE_KING);
    visit_non_capture_moves(white_kings, king_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_KING);
    visit_non_capture_ray_moves(white_queens, rook_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_QUEEN);
    visit_non_capture_ray_moves(white_rooks, rook_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_ROOK);
    visit_capture_ray_moves(white_queens, rook_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_QUEEN);
    visit_capture_ray_moves(white_rooks, rook_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_ROOK);
    visit_capture_ray_moves(white_bishops, bishop_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_BISHOP);
    visit_capture_ray_moves(white_queens, bishop_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_QUEEN);
    visit_non_capture_ray_moves(white_bishops, bishop_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_BISHOP);
    visit_non_capture_ray_moves(white_queens, bishop_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_QUEEN);
    generate_castling(f, true);
  } else {
    visit_capture_moves(black_pawns, black_pawn_capture_moves, f,
        pieces[Piece::WHITE], Piece::BLACK_PAWN);
    visit_pawn_nocaps(black_pawns, black_pawn_no_capture_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], Piece::BLACK_PAWN, false);
    visit_capture_moves(black_knights, knight_moves, f, pieces[Piece::WHITE],
        Piece::BLACK_KNIGHT);
    visit_non_capture_moves(black_knights, knight_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::BLACK_KNIGHT);
    visit_capture_moves(black_kings, king_moves, f, pieces[Piece::WHITE],
        Piece::BLACK_KING);
    visit_non_capture_moves(black_kings, king_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::BLACK_KING);
    visit_non_capture_ray_moves(black_queens, rook_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], Piece::BLACK_QUEEN);
    visit_non_capture_ray_moves(black_rooks, rook_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], Piece::BLACK_ROOK);
    visit_capture_ray_moves(black_queens, rook_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_QUEEN);
    visit_capture_ray_moves(black_rooks, rook_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_ROOK);
    visit_capture_ray_moves(black_bishops, bishop_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_BISHOP);
    visit_capture_ray_moves(black_queens, bishop_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_QUEEN);
    visit_non_capture_ray_moves(black_bishops, bishop_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], Piece::BLACK_BISHOP);
    visit_non_capture_ray_moves(black_queens, bishop_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], Piece::BLACK_QUEEN);
    generate_castling(f, false);

  }
//cout << "after: " << moves.size() << endl;
  return moves;
}

vector<Move> Move_generator::generate_moves(uint8_t square)
{
//  cout << "generating moves to: " << Square::mailbox_index_to_square(square)
//      << endl;
  //TODO very inefficient, also duplicated
  pieces = p->getPieceBitboards();
  vector<Move> moves;
  moves.clear();
  function<void(int8_t, uint8_t, uint8_t, int8_t)> f =
      [&moves, this, square](int8_t moving, uint8_t from, uint8_t to, int8_t captured) {
        //cout << "trying: " << m.to_string() << endl;
        if (to == square) {
          Move m (moving, from, to, captured, false);
          //cout << "found one" << endl;
          moves.push_back(m);
        }
      };

  //TODO generalize, obviously
  bb white_pawns = pieces[Piece::PAWN] & pieces[Piece::WHITE];
  bb white_knights = pieces[Piece::KNIGHT] & pieces[Piece::WHITE];
  bb white_bishops = pieces[Piece::BISHOP] & pieces[Piece::WHITE];
  bb white_rooks = pieces[Piece::ROOK] & pieces[Piece::WHITE];
  bb white_queens = pieces[Piece::QUEEN] & pieces[Piece::WHITE];
  bb white_kings = pieces[Piece::KING] & pieces[Piece::WHITE];
  bb black_pawns = pieces[Piece::PAWN] & pieces[Piece::BLACK];
  bb black_knights = pieces[Piece::KNIGHT] & pieces[Piece::BLACK];
  bb black_bishops = pieces[Piece::BISHOP] & pieces[Piece::BLACK];
  bb black_rooks = pieces[Piece::ROOK] & pieces[Piece::BLACK];
  bb black_queens = pieces[Piece::QUEEN] & pieces[Piece::BLACK];
  bb black_kings = pieces[Piece::KING] & pieces[Piece::BLACK];
  ///

  if (p->white_to_move) {
    visit_capture_moves(white_pawns, white_pawn_capture_moves, f,
        pieces[Piece::BLACK], Piece::WHITE_PAWN);
    visit_capture_moves(white_knights, knight_moves, f, pieces[Piece::BLACK],
        Piece::WHITE_KNIGHT);
    visit_capture_moves(white_kings, king_moves, f, pieces[Piece::BLACK],
        Piece::WHITE_KING);
    visit_capture_ray_moves(white_queens, rook_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_QUEEN);
    visit_capture_ray_moves(white_rooks, rook_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_ROOK);
    visit_capture_ray_moves(white_bishops, bishop_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_BISHOP);
    visit_capture_ray_moves(white_queens, bishop_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_QUEEN);
  } else {
    visit_capture_moves(black_pawns, black_pawn_capture_moves, f,
        pieces[Piece::WHITE], Piece::BLACK_PAWN);
    visit_capture_moves(black_knights, knight_moves, f, pieces[Piece::WHITE],
        Piece::BLACK_KNIGHT);
    visit_capture_moves(black_kings, king_moves, f, pieces[Piece::WHITE],
        Piece::BLACK_KING);
    visit_capture_ray_moves(black_queens, rook_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_QUEEN);
    visit_capture_ray_moves(black_rooks, rook_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_ROOK);
    visit_capture_ray_moves(black_bishops, bishop_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_BISHOP);
    visit_capture_ray_moves(black_queens, bishop_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_QUEEN);
  }
  return moves;
}

bool Move_generator::is_attacked(uint8_t square)
{
  pieces = p->getPieceBitboards();
  vector<Move> attack_moves;
//  moves.clear();
  function<void(int8_t, uint8_t, uint8_t, int8_t)> f =
      [&attack_moves, this, square](int8_t moving, uint8_t from, uint8_t to, int8_t captured) {
        bool en_passant = false;
        if ((moving == Piece::WHITE_PAWN || moving == Piece::BLACK_PAWN) &&Position::is_set_square(p->en_passant_square, to)) {
          en_passant = true;
        }
        //cout << "trying: " << m.to_string() << endl;
        if (to == square) {
          Move m (moving, from, to, captured, en_passant);
          //cout << "found one" << endl;
          attack_moves.push_back(m);
        }
      };

  //TODO generalize, obviously
  bb white_pawns = pieces[Piece::PAWN] & pieces[Piece::WHITE];
  bb white_knights = pieces[Piece::KNIGHT] & pieces[Piece::WHITE];
  bb white_bishops = pieces[Piece::BISHOP] & pieces[Piece::WHITE];
  bb white_rooks = pieces[Piece::ROOK] & pieces[Piece::WHITE];
  bb white_queens = pieces[Piece::QUEEN] & pieces[Piece::WHITE];
  bb white_kings = pieces[Piece::KING] & pieces[Piece::WHITE];
  bb black_pawns = pieces[Piece::PAWN] & pieces[Piece::BLACK];
  bb black_knights = pieces[Piece::KNIGHT] & pieces[Piece::BLACK];
  bb black_bishops = pieces[Piece::BISHOP] & pieces[Piece::BLACK];
  bb black_rooks = pieces[Piece::ROOK] & pieces[Piece::BLACK];
  bb black_queens = pieces[Piece::QUEEN] & pieces[Piece::BLACK];
  bb black_kings = pieces[Piece::KING] & pieces[Piece::BLACK];
  ///

  if (p->white_to_move) {
    visit_capture_moves(white_pawns, white_pawn_capture_moves, f,
        pieces[Piece::BLACK], Piece::WHITE_PAWN);
    visit_pawn_nocaps(white_pawns, white_pawn_no_capture_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_PAWN, true);
    visit_capture_moves(white_knights, knight_moves, f, pieces[Piece::BLACK],
        Piece::WHITE_KNIGHT);
    visit_non_capture_moves(white_knights, knight_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_KNIGHT);
    visit_capture_moves(white_kings, king_moves, f, pieces[Piece::BLACK],
        Piece::WHITE_KING);
    visit_non_capture_moves(white_kings, king_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_KING);
    visit_non_capture_ray_moves(white_queens, rook_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_QUEEN);
    visit_non_capture_ray_moves(white_rooks, rook_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_ROOK);
    visit_capture_ray_moves(white_queens, rook_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_QUEEN);
    visit_capture_ray_moves(white_rooks, rook_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_ROOK);
    visit_capture_ray_moves(white_bishops, bishop_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_BISHOP);
    visit_capture_ray_moves(white_queens, bishop_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], pieces[Piece::BLACK],
        Piece::WHITE_QUEEN);
    visit_non_capture_ray_moves(white_bishops, bishop_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_BISHOP);
    visit_non_capture_ray_moves(white_queens, bishop_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::WHITE_QUEEN);
    //  generate_castling(f, true);
  } else {
    visit_capture_moves(black_pawns, black_pawn_capture_moves, f,
        pieces[Piece::WHITE], Piece::BLACK_PAWN);
    visit_pawn_nocaps(black_pawns, black_pawn_no_capture_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], Piece::BLACK_PAWN, false);
    visit_capture_moves(black_knights, knight_moves, f, pieces[Piece::WHITE],
        Piece::BLACK_KNIGHT);
    visit_non_capture_moves(black_knights, knight_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::BLACK_KNIGHT);
    visit_capture_moves(black_kings, king_moves, f, pieces[Piece::WHITE],
        Piece::BLACK_KING);
    visit_non_capture_moves(black_kings, king_moves, f,
        pieces[Piece::WHITE] | pieces[Piece::BLACK], Piece::BLACK_KING);
    visit_non_capture_ray_moves(black_queens, rook_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], Piece::BLACK_QUEEN);
    visit_non_capture_ray_moves(black_rooks, rook_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], Piece::BLACK_ROOK);
    visit_capture_ray_moves(black_queens, rook_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_QUEEN);
    visit_capture_ray_moves(black_rooks, rook_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_ROOK);
    visit_capture_ray_moves(black_bishops, bishop_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_BISHOP);
    visit_capture_ray_moves(black_queens, bishop_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], pieces[Piece::WHITE],
        Piece::BLACK_QUEEN);
    visit_non_capture_ray_moves(black_bishops, bishop_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], Piece::BLACK_BISHOP);
    visit_non_capture_ray_moves(black_queens, bishop_moves, f,
        pieces[Piece::BLACK] | pieces[Piece::WHITE], Piece::BLACK_QUEEN);
    //generate_castling(f, false);

  }
  bool retval = false;
  for (auto& move : attack_moves) {
    //cout << "is attacked?: " << move.to_string() << endl;
    uint8_t t = move.get_to();
    if (t == square) {
      retval = true;
      //cout << "yes" << endl;
      break;
    }
  }
  return retval;
}

bool Move_generator::is_in_check(bool side)
{
  //  cout << "checking for side: " << (side ? "white" : "black") << endl;
//  cout << "to move: " << (p->white_to_move ? "white" : "black") << endl;
//
//TODO this is only the naive way of doing this, it needs to be much more efficient

  int king_pos = 0;
  function<void(int)> f = [&king_pos](int square) {
//    cout << "determined: " << square << endl;
      king_pos = square;
//    cout << "determined: " << king_pos << endl;
    };
  bb wk = p->kings & p->white;
  bb bk = p->kings & p->black;
//  cout << hex << p->kings << "... " << wk << dec << endl;
//  cout << hex << p->kings << "... " << bk << dec << endl;
  if (side) {
//    cout << "determining for white: ";
    Position::visit_bitboard(wk, f);
  } else {
//    cout << "determining for black: ";
    Position::visit_bitboard(bk, f);
  }
  bool retval = is_attacked(king_pos);
//  cout << "king_pos= " << king_pos << endl;
//  cout << "in check: " << retval << endl;
  return retval;
}
