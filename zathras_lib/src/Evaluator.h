#pragma once
#pragma once

#include <algorithm>

#include "Position.h"

#include "PieceCount.h"
namespace Eval {
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
		static int countSetBits(bb n) //TODO use builtin-popcount
		{
			// base case 
			if (n == 0)
				return 0;
			else
				return 1 + countSetBits(n & (n - 1));
		}
		static int getMidgameValue(Position position) {

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
			const bb occupied = p->white | p->black;
			int whiteValue = 0;
			int blackValue = 0;
			int whiteMValue = 0;
			int blackMValue = 0;
			whiteMValue += 100 * countSetBits(white_pawns);
			whiteMValue += 320 * countSetBits(white_knights);
			whiteMValue += 330 * countSetBits(white_bishops);
			whiteMValue += 500 * countSetBits(white_rooks);
			whiteMValue += 900 * countSetBits(white_queens);

			blackMValue += 100 * countSetBits(black_pawns);
			blackMValue += 320 * countSetBits(black_knights);
			blackMValue += 330 * countSetBits(black_bishops);
			blackMValue += 500 * countSetBits(black_rooks);
			blackMValue += 900 * countSetBits(black_queens);

			//int whiteCValue = 0;
			//int blackCValue = 0;
			//bool possibleBishopPairWhite = false;
			//bool possibleBishopPairBlack = false;
			//int pawnStructureValue = evaluatePawnStructureMidgame();

			//int p;
			//for (int i = 11; i < 89; ++i) {
			//	int file = i % 10;
			//	p = position.board[i];
			//	if (p == 0) {
			//		continue;
			//	}
			//	else if (p > 1) { //WHITE
			//		whiteMValue += getPieceMaterialValue(p);
			//		whiteCValue += getPieceMidgameValue(i, p, file);
			//		if (p == 3) {
			//			if (possibleBishopPairWhite) {
			//				whiteCValue += 50;
			//			}
			//			possibleBishopPairWhite = true;
			//		}
			//	}
			//	else if (p < -1) { //BLACK
			//		blackMValue += getPieceMaterialValue(p);
			//		blackCValue += getPieceMidgameValue(i, p, file);
			//		if (p == -3) {
			//			if (possibleBishopPairBlack) {
			//				blackCValue += 50;
			//			}
			//			possibleBishopPairBlack = true;
			//		}
			//	}
			//	else {
			//		//pawns, already dealt with elsewhere
			//	}
			//}


			//blackValue += blackCValue;
			//whiteValue += whiteCValue;

			///*if (!position.hasCastledBlack)
			//	blackValue -= 61;
			//if (!position.hasCastledWhite)
			//	whiteValue -= 61;*/
			//if (whiteMValue > blackMValue + 105) {
			//	int diff = whiteMValue + blackMValue - 500;
			//	int bonus = (8000 - diff) / 100;
			//	if (bonus > 74)
			//		bonus = 74;
			//	whiteValue += bonus;
			//}
			//else if (blackMValue > whiteMValue + 105) {
			//	int diff = blackMValue + whiteMValue - 500;
			//	int bonus = (8000 - diff) / 100;
			//	if (bonus > 74)
			//		bonus = 74;
			//	blackValue += bonus;
			//}
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
		static bool isInsufficientMaterial(PieceCount pc) {
			return (pc.loneKing()) || (pc.oneLight()) || (pc.isNN());
		}
		static bool isEndgame(PieceCount wpc) {
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


		static bool isInsufficientMaterial(PieceCount we, PieceCount other) {
			return (isInsufficientMaterial(we)) && (isInsufficientMaterial(other));
		}
		static int getValue(Position p) {
			//position = p;
		/*	PieceCount wpc = countWhitePieces();
			PieceCount bpc = countBlackPieces();
			if (isInsufficientMaterial(wpc, bpc))
				return 0;
			if (isInsufficientMaterial(bpc, wpc))
				return 0;
			if (isEndgame(wpc) || isEndgame(bpc)) {
				return getEndgameValue(wpc, bpc);
			}*/
			return getMidgameValue(p);
		}

	};

}