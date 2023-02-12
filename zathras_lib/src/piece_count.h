#pragma once
namespace Eval {
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
		bool loneKing()
		{
			if (!lightsOnly())
				return false;
			return (knightsCount == 0) && (bishopsCount == 0);
		}

		bool oneLight()
		{
			if (!lightsOnly())
				return false;
			if ((knightsCount == 1) && (bishopsCount == 0))
				return true;
			return (knightsCount == 0) && (bishopsCount == 1);
		}

		bool lightsOnly()
		{
			return (pawnsCount <= 0) && (rookCount <= 0) && (queensCount <= 0);
		}

		bool isNN()
		{
			if (!lightsOnly())
				return false;
			return (knightsCount == 2) && (bishopsCount == 0);
		}

		bool isKBNK()
		{
			if (!lightsOnly())
				return false;
			return (knightsCount == 1) && (bishopsCount == 1);
		}
	};
}