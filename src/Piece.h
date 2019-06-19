/*
 * Piece.h
 *
 *  Created on: Apr 23, 2016
 *      Author: nczempin
 */

#ifndef PIECE_H_
#define PIECE_H_

class Piece
{
public:
  Piece();
  virtual ~Piece();
  static const int PAWN = 1; //TODO enums
  static const int KNIGHT = 2;
  static const int BISHOP = 3;
  static const int ROOK = 4;
  static const int QUEEN = 5;
  static const int KING = 6;
  static const int WHITE_PAWN = 1;
  static const int WHITE_KNIGHT = 2;
  static const int WHITE_BISHOP = 3;
  static const int WHITE_ROOK = 4;
  static const int WHITE_QUEEN = 5;
  static const int WHITE_KING = 6;
  static const int WHITE = 7;
  static const int BLACK_PAWN = -1;
  static const int BLACK_KNIGHT = -2;
  static const int BLACK_BISHOP = -3;
  static const int BLACK_ROOK = -4;
  static const int BLACK_QUEEN = -5;
  static const int BLACK_KING = -6;
  static const int BLACK = 8;

};

#endif /* PIECE_H_ */
