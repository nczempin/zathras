#pragma once
namespace Eval {
	// Legacy piece-count summary ported from Eden (see Issue #142).
	// Note on FIDE dead positions vs. practical draw heuristics:
	//   - loneKing() (K) and oneLight() (K+N or K+B) cannot force mate and,
	//     against a lone king, are strict FIDE dead positions (Art. 5.2.2).
	//   - isNN() (K+N+N) is NOT a FIDE dead position (helpmate exists against a
	//     lone king, and K+N+N can even force mate in some positions when the
	//     defender has an extra minor piece/pawn that prevents stalemate), but
	//     K+N+N vs K is a practical draw (~99.999999%) against non-cooperative play.
	class PieceCount {
	public:
		int pawnsCount;
		int knightsCount;
		int bishopsCount;
		int rookCount;
		int queensCount;


		PieceCount(int pawnsCount, int knightsCount, int bishopsCount, int rookCount, int queensCount) {
			this->pawnsCount = pawnsCount;
			this->knightsCount = knightsCount;
			this->bishopsCount = bishopsCount;
			this->rookCount = rookCount;
			this->queensCount = queensCount;
		}
		bool loneKing() const
		{
			if (!lightsOnly())
				return false;
			return (knightsCount == 0) && (bishopsCount == 0);
		}

		bool oneLight() const
		{
			if (!lightsOnly())
				return false;
			if ((knightsCount == 1) && (bishopsCount == 0))
				return true;
			return (knightsCount == 0) && (bishopsCount == 1);
		}

		bool lightsOnly() const
		{
			return (pawnsCount <= 0) && (rookCount <= 0) && (queensCount <= 0);
		}

		bool isNN() const
		{
			if (!lightsOnly())
				return false;
			return (knightsCount == 2) && (bishopsCount == 0);
		}

		bool isKBNK() const
		{
			if (!lightsOnly())
				return false;
			return (knightsCount == 1) && (bishopsCount == 1);
		}
	};
}