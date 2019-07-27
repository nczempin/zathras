#include "Position.h"
namespace Positions {
	Position Position::create_position(const string& fen) {
		Position position;
		vector<string> split_fen = Util::split(fen, ' ');
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
				//cout << "right: " << right << endl;
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
		vector<string> ranks = Util::split(fen_board, '/');
		int r = 7;
		fill_n(position.board, 64, 0);
		for (auto& rank : ranks) {
			int f = 0;
			for (auto& c : rank) {
				if (Util::is_digit(c)) {
					int digit = c - '0'; //convert from ascii
					f += digit;
				}
				else {
					switch (c) {
					case 'P':
						set_square(f, r, position.pawns);
						set_square(f, r, position.white);
						position.board[f + r * 8] = Piece::WHITE_PAWN; //TODO encapsulate
						break;
					case 'N':
						set_square(f, r, position.knights);
						set_square(f, r, position.white);
						position.board[f + r * 8] = Piece::WHITE_KNIGHT; //TODO encapsulate
						break;
					case 'B':
						set_square(f, r, position.bishops);
						set_square(f, r, position.white);
						position.board[f + r * 8] = Piece::WHITE_BISHOP; //TODO encapsulate
						break;
					case 'R':
						set_square(f, r, position.rooks);
						set_square(f, r, position.white);
						position.board[f + r * 8] = Piece::WHITE_ROOK; //TODO encapsulate
						break;
					case 'Q':
						set_square(f, r, position.queens);
						set_square(f, r, position.white);
						position.board[f + r * 8] = Piece::WHITE_QUEEN; //TODO encapsulate
						break;
					case 'K':
						set_square(f, r, position.kings);
						set_square(f, r, position.white);
						position.board[f + r * 8] = Piece::WHITE_KING; //TODO encapsulate
						break;
					case 'p':
						set_square(f, r, position.pawns);
						set_square(f, r, position.black);
						position.board[f + r * 8] = Piece::BLACK_PAWN; //TODO encapsulate
						break;
					case 'n':
						set_square(f, r, position.knights);
						set_square(f, r, position.black);
						position.board[f + r * 8] = Piece::BLACK_KNIGHT; //TODO encapsulate
						break;
					case 'b':
						set_square(f, r, position.bishops);
						set_square(f, r, position.black);
						position.board[f + r * 8] = Piece::BLACK_BISHOP; //TODO encapsulate
						break;
					case 'r':
						set_square(f, r, position.rooks);
						set_square(f, r, position.black);
						position.board[f + r * 8] = Piece::BLACK_ROOK; //TODO encapsulate
						break;
					case 'q':
						set_square(f, r, position.queens);
						set_square(f, r, position.black);
						position.board[f + r * 8] = Piece::BLACK_QUEEN; //TODO encapsulate
						break;
					case 'k':
						set_square(f, r, position.kings);
						set_square(f, r, position.black);
						position.board[f + r * 8] = Piece::BLACK_KING; //TODO encapsulate
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
		return position;
	}

	Position Position::create_start_position() {
		const char* p = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";


		Position start_position = create_position(p);
		return start_position;
	}
}