#pragma once

#include <algorithm>

#include "Position.h"

#include "PieceCount.h"
namespace Eval {
	// NOTE: Commented-out 10x10 mailbox evaluation blocks in this header are legacy
	// remnants from the Java Eden port; tracked for cleanup in Issue #138.
	class Evaluator
	{
	public:
		Evaluator(void);
		virtual ~Evaluator(void);
		//Position position;



		static int pieceValues[];

		static int bishopSquareValues[];

		static int pawnSquareValues[];

		static int kingEndgameSquareValues[];

		static int kingEndgameSquareValuesKBNKLight[];

		static int kingEndgameSquareValuesKBNKDark[];

		static int knightSquareValues[];

		static int passedPawnProgression[];
		/*static int getPieceEndgameValue(int square, int piece, int file) {
			int retValue = 0;
			switch (piece) {
			case -6:

			case 6:
				retValue = getKingEndgameValue(square, piece);
				break;

			case -5:

			case 5:
				retValue = -25;
				break;

			case -4:

			case 4:
				retValue = 25;
				break;

			case -3:

			case 3:
				retValue = 3;
				break;

			case -2:

			case 2:
				retValue = -3;
				break;

			case -1:

			case 1:
				retValue = getPawnEndgameValue(square, piece, file);
				break;

			case 0:

			default:
				retValue = 0;
			}

			return retValue;
		}*/

		static int getPieceMaterialValue(int piece) {
			int retVal = pieceValues[(abs(piece) - 1)];
			return retVal;
		}
		/*
			static int getPieceMidgameValue(int square, int piece, int file) {
				int retValue = 0;
				switch (piece) {
				case -6:

				case 6:
					retValue = getKingMidgameValue(square, piece, file);
					break;

				case -5:

				case 5:
					retValue = getQueenValue(piece, square, file);
					break;

				case -4:

				case 4:
					retValue = getRookValue(piece, square, file);
					break;

				case -3:

				case 3:
					retValue = getBishopValue(square, piece);
					break;

				case -2:

				case 2:
					retValue = getKnightValue(piece, square);
					break;

				case -1:

				case 1:
					retValue = getPawnMidgameValue(square, piece, file);
					break;

				case 0:

				default:
					retValue = 0;
				}
				return retValue;
			}

			static int getQueenValue(int plusminus, int square, int file) {
				int retValue = getRookValue(plusminus, square, file) / 4;
				return retValue;
			}*/

			/*static int getHorizontalKingTropism(int plusminus, int square) {
				int kingPosition;

				if (plusminus > 0) {
					kingPosition = position.blackKing;
				}
				else
					kingPosition = position.whiteKing;
				int myRank = square % 10;
				int yourRank = kingPosition % 10;
				return abs(myRank - yourRank);
			}*/
			/*static int getVerticalKingTropism(int plusminus, int square) {
				int kingPosition;

				if (plusminus > 0) {
					kingPosition = position.blackKing;
				}
				else
					kingPosition = position.whiteKing;
				int myRank = square / 10;
				int yourRank = kingPosition / 10;
				return abs(myRank - yourRank);
			}*/

			//static int getRookValue(int plusminus, int square, int file) {
			//	int retVal = isOnOpenFile(square, file) ? 12 : 0;
			//	/*int x = getHorizontalKingTropism(plusminus, square);
			//	int y = getVerticalKingTropism(plusminus, square);
			//	int minTropism = min(x, y);
			//	retVal -= minTropism / 4;*/
			//	return retVal;
			//}
			//static int getKingEndgameValue(int square, int piece) {
			//	return kingEndgameSquareValues[(square - 10)];
			//}

			//static int getKingMidgameValue(int square, int piece, int file) {
			//	int retValue = 0;
			//	int openFiles = 0;
			//	int halfOpenFiles = 0;
			//	int rank = square / 10;
			//	if (isOnOpenFile(square, file)) {
			//		openFiles++;
			//	}
			//	else if (isOnOpenOrHalfOpenFile(piece, file, rank))
			//		halfOpenFiles++;
			//	if (isOnOpenFile(square, file - 1)) {
			//		openFiles++;
			//	}
			//	else if (isOnOpenOrHalfOpenFile(piece, file - 1, rank))
			//		halfOpenFiles++;
			//	if (isOnOpenFile(square, file + 1)) {
			//		openFiles++;
			//	}
			//	else if (isOnOpenOrHalfOpenFile(piece, file + 1, rank))
			//		halfOpenFiles++;
			//	retValue = (int)(retValue - 30.0 * pow(2.0, openFiles));
			//	retValue = (int)(retValue - 5.0 * pow(2.0, halfOpenFiles));
			//	if (piece == 6) {
			//		if ((square > 13) && (square < 17))
			//			retValue -= 35;
			//		if (square > 20) {
			//			retValue -= 55;
			//		}
			//		else {
			//			if ((square == 17) || (square == 18)) {
			//				if (position.board[26] != 1)
			//					retValue -= 10;
			//				if ((position.board[27] != 1) && (position.board[27] != 3)) {
			//					retValue -= 10;
			//					if (position.board[37] != 1)
			//						retValue -= 10;
			//				}
			//				if (position.board[28] != 1) {
			//					retValue -= 6;
			//					if (position.board[38] != 1) {
			//						retValue -= 11;
			//					}
			//				}
			//			}
			//			else if ((square == 12) || (square == 11) || (square == 13)) {
			//				if (position.board[23] != 1)
			//					retValue -= 10;
			//				if ((position.board[22] != 1) && (position.board[22] != 3)) {
			//					retValue -= 10;
			//					if (position.board[32] != 1)
			//						retValue -= 10;
			//				}
			//				if (position.board[21] != 1) {
			//					retValue -= 10;
			//					if (position.board[31] != 1)
			//						retValue -= 10;
			//				}
			//			}
			//			if (square == 13)
			//				retValue -= 37;
			//		}
			//	}
			//	if (piece == -6) {
			//		if ((square > 83) && (square < 87))
			//			retValue -= 35;
			//		if (square < 80) {
			//			retValue -= 55;
			//		}
			//		else if ((square == 87) || (square == 88)) {
			//			if (position.board[76] != -1)
			//				retValue -= 10;
			//			if ((position.board[77] != -1) && (position.board[77] != -3)) {
			//				retValue -= 10;
			//				if (position.board[67] != -1)
			//					retValue -= 10;
			//			}
			//			if (position.board[78] != -1) {
			//				retValue -= 6;
			//				if (position.board[68] != -1) {
			//					retValue -= 11;
			//				}
			//			}
			//		}
			//		else if ((square == 82) || (square == 81) || (square == 83)) {
			//			if (position.board[73] != -1)
			//				retValue -= 10;
			//			if ((position.board[72] != -1) || (position.board[72] != -3)) {
			//				retValue -= 10;
			//				if (position.board[62] != -1)
			//					retValue -= 10;
			//			}
			//			if (position.board[71] != -1) {
			//				retValue -= 10;
			//				if (position.board[61] != -1)
			//					retValue -= 10;
			//			}
			//			if (square == 83)
			//				retValue -= 37;
			//		}
			//	}
			//	return retValue;
			//}

			//static	int getKnightValue(int piece, int square) {
			//	int retVal = knightSquareValues[(square - 10)];
			//	/*int x = getHorizontalKingTropism(piece, square);
			//	int y = getVerticalKingTropism(piece, square);
			//	int manhattanTropism = x + y;
			//	retVal -= manhattanTropism / 3;*/
			//	return retVal;
			//}
			//static 	int getBishopValue(int i, int plusminus) {
			//	int retValue = 0;
			//	if (plusminus > 0) {
			//		retValue += bishopSquareValues[(i - 10)];
			///*		if (i == 16) {
			//			if (position.board[25] > 0)
			//				retValue -= 3;
			//			if (position.board[27] > 0)
			//				retValue--;
			//		}
			//		if ((i == 34) && (position.board[24] == 1)) {
			//			retValue -= 25;
			//		}*/
			//	}
			//	else {
			//		retValue += bishopSquareValues[(89 - i)];
			//		//if (i == 86) {
			//		//	if (position.board[75] < 0)
			//		//		retValue -= 3;
			//		//	if (position.board[77] < 0)
			//		//		retValue--;
			//		//}
			//		//if ((i == 64) && (position.board[74] == -1))
			//		//	retValue -= 25;
			//	}
			//	return retValue;
			//}
		static int countSetBits(bb n)
		{
#if defined(__GNUC__) || defined(__clang__)
			return __builtin_popcountll(n);
#else
			int count = 0;
			while (n) {
				n &= (n - 1);
				++count;
			}
			return count;
#endif
		}
		static int getMidgameValue(const Position& position) {
			static const int pawn_pst[64] = {
				  0,   0,   0,   0,   0,   0,   0,   0,
				 50,  50,  50,  50,  50,  50,  50,  50,
				 10,  10,  20,  30,  30,  20,  10,  10,
				  5,   5,  10,  25,  25,  10,   5,   5,
				  0,   0,   0,  20,  20,   0,   0,   0,
				  5,  -5, -10,   0,   0, -10,  -5,   5,
				  5,  10,  10, -20, -20,  10,  10,   5,
				  0,   0,   0,   0,   0,   0,   0,   0
			};
			static const int knight_pst[64] = {
				-50, -40, -30, -30, -30, -30, -40, -50,
				-40, -20,   0,   0,   0,   0, -20, -40,
				-30,   0,  10,  15,  15,  10,   0, -30,
				-30,   5,  15,  20,  20,  15,   5, -30,
				-30,   0,  15,  20,  20,  15,   0, -30,
				-30,   5,  10,  15,  15,  10,   5, -30,
				-40, -20,   0,   5,   5,   0, -20, -40,
				-50, -40, -30, -30, -30, -30, -40, -50
			};
			static const int bishop_pst[64] = {
				-20, -10, -10, -10, -10, -10, -10, -20,
				-10,   0,   0,   0,   0,   0,   0, -10,
				-10,   0,   5,  10,  10,   5,   0, -10,
				-10,   5,   5,  10,  10,   5,   5, -10,
				-10,   0,  10,  10,  10,  10,   0, -10,
				-10,  10,  10,  10,  10,  10,  10, -10,
				-10,   5,   0,   0,   0,   0,   5, -10,
				-20, -10, -10, -10, -10, -10, -10, -20
			};
			static const int rook_pst[64] = {
				  0,   0,   0,   0,   0,   0,   0,   0,
				  5,  10,  10,  10,  10,  10,  10,   5,
				 -5,   0,   0,   0,   0,   0,   0,  -5,
				 -5,   0,   0,   0,   0,   0,   0,  -5,
				 -5,   0,   0,   0,   0,   0,   0,  -5,
				 -5,   0,   0,   0,   0,   0,   0,  -5,
				 -5,   0,   0,   0,   0,   0,   0,  -5,
				  0,   0,   0,   5,   5,   0,   0,   0
			};
			static const int queen_pst[64] = {
				-20, -10, -10,  -5,  -5, -10, -10, -20,
				-10,   0,   0,   0,   0,   0,   0, -10,
				-10,   0,   5,   5,   5,   5,   0, -10,
				 -5,   0,   5,   5,   5,   5,   0,  -5,
				  0,   0,   5,   5,   5,   5,   0,  -5,
				-10,   5,   5,   5,   5,   5,   0, -10,
				-10,   0,   5,   0,   0,   0,   0, -10,
				-20, -10, -10,  -5,  -5, -10, -10, -20
			};
			static const int king_mg_pst[64] = {
				-30, -40, -40, -50, -50, -40, -40, -30,
				-30, -40, -40, -50, -50, -40, -40, -30,
				-30, -40, -40, -50, -50, -40, -40, -30,
				-30, -40, -40, -50, -50, -40, -40, -30,
				-20, -30, -30, -40, -40, -30, -30, -20,
				-10, -20, -20, -20, -20, -20, -20, -10,
				 20,  20,   0,   0,   0,   0,  20,  20,
				 20,  30,  10,   0,   0,  10,  30,  20
			};
			static const int king_eg_pst[64] = {
				-50, -40, -30, -20, -20, -30, -40, -50,
				-30, -20, -10,   0,   0, -10, -20, -30,
				-30, -10,  20,  30,  30,  20, -10, -30,
				-30, -10,  30,  40,  40,  30, -10, -30,
				-30, -10,  30,  40,  40,  30, -10, -30,
				-30, -10,  20,  30,  30,  20, -10, -30,
				-30, -30,   0,   0,   0,   0, -30, -30,
				-50, -30, -30, -30, -30, -30, -30, -50
			};

			const Position* p = &position;
			const bb white_pawns = p->pawns & p->white;
			const bb white_knights = p->knights & p->white;
			const bb white_bishops = p->bishops & p->white;
			const bb white_rooks = p->rooks & p->white;
			const bb white_queens = p->queens & p->white;
			const bb white_kings = p->kings & p->white;
			const bb black_pawns = p->pawns & p->black;
			const bb black_knights = p->knights & p->black;
			const bb black_bishops = p->bishops & p->black;
			const bb black_rooks = p->rooks & p->black;
			const bb black_queens = p->queens & p->black;
			const bb black_kings = p->kings & p->black;

			int wp = countSetBits(white_pawns), bp = countSetBits(black_pawns);
			int wn = countSetBits(white_knights), bn = countSetBits(black_knights);
			int wb = countSetBits(white_bishops), bb_cnt = countSetBits(black_bishops);
			int wr = countSetBits(white_rooks), br = countSetBits(black_rooks);
			int wq = countSetBits(white_queens), bq = countSetBits(black_queens);

			// Practical minor-piece draw shortcut (see Issue #142):
			// When neither side has any pawns, rooks, or queens and each side has at most
			// one minor piece, return 0.
			// Note that under FIDE Laws of Chess (Art. 5.2.2), only K vs K, KN vs K,
			// KB vs K, and KB vs KB with same-colored bishops are strictly dead positions;
			// KN vs KN, KB vs KN, and opposite-colored bishops have legal corner helpmates
			// (and rare 1-ply mates if a king is already self-trapped in a corner).
			// Scoring all <=1-minor-per-side pawnless positions as 0 is a practical
			// "99.999999% draw" heuristic so the engine does not hallucinate a +-320/330 cp
			// winning advantage in unwinnable minor-piece endgames.
			if (wp == 0 && bp == 0 && wr == 0 && br == 0 && wq == 0 && bq == 0) {
				if (wn + wb <= 1 && bn + bb_cnt <= 1) {
					return 0;
				}
			}

			int whiteMValue = 100 * wp + 320 * wn + 330 * wb + 500 * wr + 900 * wq;
			int blackMValue = 100 * bp + 320 * bn + 330 * bb_cnt + 500 * br + 900 * bq;
			if (wb >= 2) whiteMValue += 30;
			if (bb_cnt >= 2) blackMValue += 30;

			int phase = (wn + bn + wb + bb_cnt) + 2 * (wr + br) + 4 * (wq + bq);
			if (phase > 24) phase = 24;

			auto sum_pst_w = [](bb b, const int* table) {
				int s = 0;
				while (b) {
					uint8_t sq = Positions::Bitboard::extract_and_remove_square(b);
					s += table[sq ^ 56];
				}
				return s;
			};
			auto sum_pst_b = [](bb b, const int* table) {
				int s = 0;
				while (b) {
					uint8_t sq = Positions::Bitboard::extract_and_remove_square(b);
					s += table[sq];
				}
				return s;
			};

			static constexpr bb FILE_A = 0x0101010101010101ULL;
			static const int passed_bonus[8] = { 0, 5, 10, 20, 35, 60, 100, 0 };

			int w_doubled = countSetBits(white_pawns & (white_pawns << 8));
			int b_doubled = countSetBits(black_pawns & (black_pawns >> 8));
			int whitePos = -w_doubled * 12
				+ sum_pst_w(white_knights, knight_pst)
				+ sum_pst_w(white_bishops, bishop_pst)
				+ sum_pst_w(white_queens, queen_pst);
			int blackPos = -b_doubled * 12
				+ sum_pst_b(black_knights, knight_pst)
				+ sum_pst_b(black_bishops, bishop_pst)
				+ sum_pst_b(black_queens, queen_pst);

			bb b = white_pawns;
			while (b) {
				uint8_t sq = Positions::Bitboard::extract_and_remove_square(b);
				whitePos += pawn_pst[sq ^ 56];
				int rank = sq >> 3;
				int file = sq & 7;
				bb adj_files = (FILE_A << file)
					| (file > 0 ? (FILE_A << (file - 1)) : 0ULL)
					| (file < 7 ? (FILE_A << (file + 1)) : 0ULL);
				bb ahead_mask = (rank < 7) ? (~0ULL << ((rank + 1) * 8)) : 0ULL;
				if ((black_pawns & adj_files & ahead_mask) == 0) {
					whitePos += passed_bonus[rank];
				}
			}
			b = black_pawns;
			while (b) {
				uint8_t sq = Positions::Bitboard::extract_and_remove_square(b);
				blackPos += pawn_pst[sq];
				int rank = sq >> 3;
				int file = sq & 7;
				bb adj_files = (FILE_A << file)
					| (file > 0 ? (FILE_A << (file - 1)) : 0ULL)
					| (file < 7 ? (FILE_A << (file + 1)) : 0ULL);
				bb ahead_mask = (rank > 0) ? ((1ULL << (rank * 8)) - 1ULL) : 0ULL;
				if ((white_pawns & adj_files & ahead_mask) == 0) {
					blackPos += passed_bonus[7 - rank];
				}
			}
			b = white_rooks;
			while (b) {
				uint8_t sq = Positions::Bitboard::extract_and_remove_square(b);
				whitePos += rook_pst[sq ^ 56];
				bb fmask = FILE_A << (sq & 7);
				if ((white_pawns & fmask) == 0) {
					whitePos += ((black_pawns & fmask) == 0) ? 20 : 10;
				}
			}
			b = black_rooks;
			while (b) {
				uint8_t sq = Positions::Bitboard::extract_and_remove_square(b);
				blackPos += rook_pst[sq];
				bb fmask = FILE_A << (sq & 7);
				if ((black_pawns & fmask) == 0) {
					blackPos += ((white_pawns & fmask) == 0) ? 20 : 10;
				}
			}

			uint8_t wk_sq = white_kings ? Positions::Bitboard::extract_square(white_kings) : 4;
			uint8_t bk_sq = black_kings ? Positions::Bitboard::extract_square(black_kings) : 60;
			int wk_score = (king_mg_pst[wk_sq ^ 56] * phase + king_eg_pst[wk_sq ^ 56] * (24 - phase)) / 24;
			int bk_score = (king_mg_pst[bk_sq] * phase + king_eg_pst[bk_sq] * (24 - phase)) / 24;

			int whiteValue = whitePos + wk_score;
			int blackValue = blackPos + bk_score;

			// Endgame mop-up when clearly winning
			if (phase <= 6) {
				int wk_r = wk_sq / 8, wk_f = wk_sq % 8;
				int bk_r = bk_sq / 8, bk_f = bk_sq % 8;
				int king_dist = std::abs(wk_r - bk_r) + std::abs(wk_f - bk_f);
				if (whiteMValue >= blackMValue + 300) {
					int bk_center = std::max(3 - bk_r, bk_r - 4) + std::max(3 - bk_f, bk_f - 4);
					whiteValue += bk_center * 15 + (14 - king_dist) * 8;
				} else if (blackMValue >= whiteMValue + 300) {
					int wk_center = std::max(3 - wk_r, wk_r - 4) + std::max(3 - wk_f, wk_f - 4);
					blackValue += wk_center * 15 + (14 - king_dist) * 8;
				}
			}

			int retVal = whiteValue + whiteMValue - blackValue - blackMValue;
			if (!position.is_white_to_move())
				retVal = -retVal;
			return retVal;
		}

		/*static int getPawnEndgameValue(int i, int plusminus, int file) {
			int retValue = 0;
			if (plusminus > 0) {
				retValue = pawnSquareValues[(79 - i)] / 2;
			}
			else
				retValue = pawnSquareValues[(i - 20)] / 2;
			int rank = i / 10;
			if (isPassed(i, plusminus, file, rank)) {
				if (plusminus > 0) {
					retValue = (int)(retValue + 1.5 * passedPawnProgression[(rank - 2)]);
				}
				else {
					retValue = (int)(retValue + 1.5 * passedPawnProgression[(7 - rank)]);
				}
			}
			if (isIsolated(i, file)) {
				retValue -= 15;
			}
			if (isDoubled(i, file)) {
				retValue -= 18;
			}
			return retValue;
		}
		static bool hasNoEnemyPawns(int piece, int file) {
			if ((file < 1) || (file > 8)) {
				return true;
			}
			int plusminus = abs(piece) / piece;
			int conversion = 80 + file;
			int start = file + 10;
			for (int i = start; i < conversion; i += 10) {
				if (position.board[i] == -plusminus)
					return false;
			}
			return true;
		}
	*/
	/*static bool hasNoEnemyPawnsAhead(int piece, int file, int rank) {
		if ((file < 1) || (file > 8))
			return true;
		int plusminus = abs(piece) / piece;
		int direction = plusminus * 10;
		int conversion = plusminus <= 0 ? 10 + file : 80 + file;
		int start = rank * 10 + file + direction;
		for (int i = start; i != conversion; i += direction) {
			if (position.board[i] == -plusminus)
				return false;
		}
		return true;
	}
	*/
	/*static bool hasNoPawns(int square, int file) {
		if ((file < 1) || (file > 8))
			return true;
		for (int i = 20 + file; i < 79; i += 10) {
			if (position.board[i] == position.board[square])
				return false;
		}
		return true;
	}
	*/

	/*static bool isIsolated(int square, int file) {
		return (hasNoPawns(square, file - 1)) && (hasNoPawns(square, file + 1));
	}*/
	/*static bool isOnOpenFile(int square, int file) {
		if ((file < 1) || (file > 8))
			return false;
		for (int i = 20 + file; i < 80; i += 10) {
			if ((position.board[i] == 1) || (position.board[i] == -1))
				return false;
		}
		return true;
	}*/
	/*
		static bool isOnOpenOrHalfOpenFile(int plusminus, int file, int rank) {
			if ((file < 1) || (file > 8))
				return false;
			if (hasNoEnemyPawns(plusminus, file))
				return true;
			return hasNoEnemyPawns(-plusminus, file);
		}
	*/
	/*static bool isPassed(int square, int plusminus, int file, int rank) {
		if (!hasNoEnemyPawnsAhead(plusminus, file - 1, rank))
			return false;
		if (!hasNoEnemyPawnsAhead(plusminus, file + 1, rank))
			return false;
		return hasNoEnemyPawnsAhead(plusminus, file, rank);
	}
	static bool isDoubled(int square, int file) {
		for (int i = 20 + file; i < 79; i += 10) {
			if ((i != square) && (position.board[i] == position.board[square]))
				return true;
		}
		return false;
	}*/

		static int getPawnMidgameValue(int i, int plusminus, int file) {
			int retValue = 0;;
			int rank = i / 10;
			if (plusminus > 0) {
				retValue = pawnSquareValues[(7 - rank) * 10 + file];
			}
			else {
				retValue = pawnSquareValues[(rank - 2) * 10 + file];
			}
			/*if (isPassed(i, plusminus, file, rank)) {
				int passedValue = 0;
				if (plusminus > 0) {
					passedValue += passedPawnProgression[(rank - 2)];
				}
				else
					passedValue += passedPawnProgression[(7 - rank)];
				retValue += passedValue;
			}
			if (isIsolated(i, file))
				retValue -= 12;
			if (isDoubled(i, file))
				retValue -= 16;*/
			return retValue;
		}
		/*static PieceCount countWhitePieces() {
			int queensCount = 0;
			int rookCount = 0;
			int knightsCount = 0;
			int bishopsCount = 0;
			int pawnsCount = 0;
			int count = 0;
			for (int i = 0; i < 89; ++i) {
				int piece = position.board[i];
				if (piece > 0) {
					count++;
				}
				switch (piece) {
				case 1:
					pawnsCount++;
					break;

				case 4:
					rookCount++;
					break;

				case 3:
					bishopsCount++;
					break;

				case 2:
					knightsCount++;
					break;

				case 5:
					queensCount++;
				}
			}

			PieceCount wpc(pawnsCount, knightsCount, bishopsCount, rookCount, queensCount);
			return wpc;
		}
		static PieceCount countBlackPieces() {
			int queensCount = 0;
			int rookCount = 0;
			int knightsCount = 0;
			int bishopsCount = 0;
			int pawnsCount = 0;
			int count = 0;
			for (int i = 0; i < 89; ++i) {
				int piece = position.board[i];
				if (piece < 0) {
					count++;
				}
				switch (-piece) {
				case 1:
					pawnsCount++;
					break;

				case 4:
					rookCount++;
					break;

				case 3:
					bishopsCount++;
					break;

				case 2:
					knightsCount++;
					break;

				case 5:
					queensCount++;
				}

			}

			PieceCount wpc(pawnsCount, knightsCount, bishopsCount, rookCount, queensCount);
			return wpc;
		}*/
		// Legacy Eden helper (currently unused by getMidgameValue; see Issue #142).
		// Treats K, KN, KB, and KNN (`isNN()`) as insufficient mating material on a
		// single-side basis. Note that KNN vs K is a practical draw heuristic rather
		// than a FIDE dead position (helpmate is legal), and KNN vs KN/KB is not even
		// a guaranteed tablebase draw in all positions because the defender's minor
		// piece can prevent stalemate and allow forced mate.
		static bool isInsufficientMaterial(const PieceCount& pc) {
			return (pc.loneKing()) || (pc.oneLight()) || (pc.isNN());
		}
		static bool isEndgame(const PieceCount& wpc) {
			//    		if ((isEndGame != null) && (isEndGame.equals(bool.TRUE)))
			//    			return true;
			int lightPiecesCount = wpc.knightsCount + wpc.bishopsCount;
			//int heavyPiecesCount = wpc.rookCount + wpc.queensCount;
			if (wpc.queensCount >= 1) {
				if (wpc.rookCount > 1)
					return false;
				if ((wpc.rookCount == 1) && (lightPiecesCount > 0))
					return false;
				return lightPiecesCount <= 2;
			}
			if ((wpc.rookCount > 1) && (lightPiecesCount > 2))
				return false;
			if ((wpc.rookCount == 1) && (lightPiecesCount > 3)) {
				return false;
			}

			//isEndGame = bool.TRUE;
			return true;
		}

		//static	int evaluatePawnStructureMidgame() {
		//	//    		Info.pawnStructureProbes += 1;
		//	//    		Long pz = getPawnZobrist();
		//	//    		if (pawnHash.containsKey(pz)) {
		//	//    			Integer value = (Integer) pawnHash.get(pz);
		//	//    			Info.pawnStructureHits += 1;
		//	//    			return value.intValue();
		//	//    		}
		//	int whiteCValue = 0;
		//	int whiteMValue = 0;
		//	int blackCValue = 0;
		//	int blackMValue = 0;
		//	for (int i = 11; i < 89; ++i) {
		//		int p = position.board[i];

		//		if (p == 1) { //WHITE
		//			whiteMValue += pieceValues[0];
		//			int file = i % 10;
		//			whiteCValue += getPawnMidgameValue(i, p, file);
		//		}
		//		else if (p == -1) {
		//			blackMValue += pieceValues[0];
		//			int file = i % 10;
		//			blackCValue += getPawnMidgameValue(i, p, file);
		//		}
		//	}
		//	int retVal = whiteCValue + whiteMValue - blackCValue - blackMValue;
		//	//        Info.phSize = pawnHash.size();
		//	//        if (Info.phSize < 256000)
		//	//            pawnHash.put(pz, new Integer(retVal));
		//	return retVal;
		//}
		//static	int evaluatePawnStructureEndgame() {
		//	//    		Info.pawnStructureProbes += 1;
		//	//    		Long pz = getPawnZobrist();
		//	//    		if (pawnHash.containsKey(pz)) {
		//	//    			Integer value = (Integer) pawnHash.get(pz);
		//	//    			Info.pawnStructureHits += 1;
		//	//    			return value.intValue();
		//	//    		}
		//	int whiteCValue = 0;
		//	int whiteMValue = 0;
		//	int blackCValue = 0;
		//	int blackMValue = 0;
		//	for (int i = 11; i < 89; ++i) {
		//		int p = position.board[i];

		//		if (p == 1) { //WHITE
		//			whiteMValue += pieceValues[0];
		//			int file = i % 10;
		//			whiteCValue += getPawnEndgameValue(i, p, file);
		//		}
		//		else if (p == -1) {
		//			blackMValue += pieceValues[0];
		//			int file = i % 10;
		//			blackCValue += getPawnEndgameValue(i, p, file);
		//		}
		//	}
		//	int retVal = whiteCValue + whiteMValue - blackCValue - blackMValue;
		//	//        Info.phSize = pawnHash.size();
		//	//        if (Info.phSize < 256000)
		//	//            pawnHash.put(pz, new Integer(retVal));
		//	return retVal;
		//}
		//static int getEndgameValue(PieceCount wpc, PieceCount bpc) {
		//	int whiteValue = 0;
		//	int whiteMValue = 0;
		//	int whiteCValue = 0;
		//	//            if ((wpc.isKBNK()) || (bpc.isKBNK()))
		//	//                adjustKingSquaresForKBNK();
		//	int blackValue = 0;
		//	int blackMValue = 0;
		//	int blackCValue = 0;
		//	bool possibleBishopPairWhite = false;
		//	bool possibleBishopPairBlack = false;
		//	int pawnStructureValue = evaluatePawnStructureEndgame();
		//	int p;
		//	for (int i = 11; i < 89; ++i) {
		//		p = position.board[i];
		//		if (p == 0) {
		//			continue;
		//		}
		//		else {
		//			if (p > 0) { //WHITE
		//				if (p != 1) {
		//					whiteMValue += getPieceMaterialValue(p);
		//					if (p == 3) {
		//						if (possibleBishopPairWhite)
		//							whiteCValue += 50;
		//						possibleBishopPairWhite = true;
		//					}
		//					int file = i % 10;
		//					whiteCValue += getPieceEndgameValue(i, p, file);
		//				}
		//			}
		//			else {//BLACK
		//				if (p != -1) {
		//					blackMValue += getPieceMaterialValue(p);
		//					if (p == -3) {
		//						if (possibleBishopPairBlack)
		//							blackCValue += 50;
		//						possibleBishopPairBlack = true;
		//					}
		//					int file = i % 10;
		//					blackCValue += getPieceEndgameValue(i, p, file);
		//				}
		//			}
		//		}
		//	}
		//	blackValue += blackCValue;
		//	whiteValue += whiteCValue;
		//	if (whiteMValue > blackMValue + 105) {
		//		int diff = whiteMValue + blackMValue - 500;
		//		int bonus = (8000 - diff) / 100;
		//		if (bonus > 74)
		//			bonus = 74;
		//		whiteValue += bonus;
		//	}
		//	else if (blackMValue > whiteMValue + 105) {
		//		int diff = blackMValue + whiteMValue - 500;
		//		int bonus = (8000 - diff) / 100;
		//		if (bonus > 74)
		//			bonus = 74;
		//		blackValue += bonus;
		//	}
		//	int retVal = 0;
		//	retVal += whiteValue + whiteMValue + pawnStructureValue - blackValue - blackMValue;
		//	if (!position.onMove)
		//		retVal = -retVal;
		//	return retVal;
		//}


		static bool isInsufficientMaterial(const PieceCount& we, const PieceCount& other) {
			return (isInsufficientMaterial(we)) && (isInsufficientMaterial(other));
		}
		static int getValue(const Position& p) {
			return getMidgameValue(p);
		}

	};

}