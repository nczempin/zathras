/*
 * Move.h
 *
 *  Created on: 20.04.2016
 *      Author: nczempin
 */

#ifndef MOVE_H_
#define MOVE_H_

#include "typedefs.h"
#include "Square.h"
#include "global.h"
#include <string>
using namespace std;

class Move
{
public:
	Move();
	Move(int8_t piece, uint8_t from, uint8_t to, int8_t captured,
		bool en_passant_capture);
	Move(string move) {
		from = static_cast<square_t> (decodeSquare(move.substr(0, 2)));

		to = static_cast<square_t> (decodeSquare(move.substr(2, 4)));
		string promotedTo = move.substr(4);
		promoted_to = 0;
		if (promotedTo != "") {
			promoted_to = static_cast<piece_t> (decodePiece(promotedTo));
		}

		captured = 0;

		
	}
	inline uint8_t get_from() const {
		return this->from;
	}
	inline uint8_t get_to() const {
		return this->to;
	}
	int8_t get_moving_piece() const;

	inline void set_moving_piece(int8_t moving) {
		this->moving = moving;
	}

	void set_captured(int8_t taken);

	inline void set_from(uint8_t from) {
		this->from = from;
	}
	inline void set_to(uint8_t to) {
		this->to = to;
	}
	string to_string() const;
	int value{ 0 };
	//  uint8_t get_en_passant_square() const  {
	//    return en_passant_square;
	//  }
	//
	//  void set_en_passant_square(uint8_t en_passant){
	//    this->en_passant_square = en_passant;
	//  }



	bool is_en_passant_capture() const;
	void set_en_passant_capture(bool enPassantCapture);

	int8_t get_promoted_to() const {
		return promoted_to;
	}

	void set_promoted_to(int8_t promotedTo = 0) {
		promoted_to = promotedTo;
	}

	int8_t get_captured() const {
		return captured;
	}

	//  bool is_cleared_kingside_castling() const {
	//	  return cleared_kingside_castling;
	//  }
	//
	//  void set_cleared_kingside_castling(bool b) {
	//	  this->cleared_kingside_castling = b;
	//  }
	//  void set_cleared_queenside_castling(bool b) {
	//	  cleared_queenside_castling = b;
	//  }
	//
	//  bool is_cleared_queenside_castling() const {
	//	  return cleared_queenside_castling;
	//  }

private:
	square_t from = 0;
	square_t to = 0;
	piece_t moving = 0;
	piece_t captured = 0; //TODO maybe also move this out
	piece_t promoted_to = 0;

	bool en_passant_capture = false;

	//TODO all these are not directly part of the move, should probably put them in a separate structure
   //  square_t en_passant_square = 0; //TODO file would be sufficient
  //  bool cleared_queenside_castling = false;
  //  bool cleared_kingside_castling = false;


};

#endif /* MOVE_H_ */
