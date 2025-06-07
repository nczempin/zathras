
#ifndef BITBOARD_H_
#define BITBOARD_H_

#include <cassert>

//TODO mac
#if defined(__GNUC__)
#elif defined(_WIN64) && (_MSC_VER >= 1500)
#include <intrin.h>
#pragma intrinsic(_BitScanForward)
#else
#endif

#include "typedefs.h"
namespace positions {
	class Bitboard {
	public:
		Bitboard()noexcept;
		virtual ~Bitboard();

		static void visit_bitboard(const bb my_bb, const square_visitor& f);

		static uint8_t ffs_software(const bb& my_bb)noexcept {
			if (my_bb == 0) {
				return 0;
			}
			bb tmp = 1;
			uint8_t retval = 1;
			while (!(my_bb & tmp)) {
				tmp <<= 1;
				++retval;
			}
			return retval;
		}
#if defined(__GNUC__)
		inline static uint8_t ffs(const bb& my_bb) {
			return __builtin_ffsll(my_bb);
		}
#elif defined(_WIN64) && (_MSC_VER >= 1500) 
		inline static uint8_t ffs(const bb& my_bb)noexcept {
			assert(my_bb != 0);
			unsigned long index;
			const bool isNonZero = _BitScanForward64(&index, my_bb);
			assert(isNonZero);
			return static_cast<uint8_t> (index);
		}

#else
		inline static uint8_t ffs(const bb& my_bb) {
			return ffs_software(my_bb);
		}
#endif

		inline static constexpr uint8_t look_up1(uint8_t index) {
			//TODO eliminate entirely
			return index;
			/*uint8_t m = index - 1;
			uint8_t rank = m / 8;
			uint8_t file = m % 8;
			rank *= 8;
			file = 7 - file;
			return file + rank;*/

		}
		inline static constexpr uint8_t look_up2(uint8_t index) {
			
			uint8_t m = index - 1;
			uint8_t rank = m / 8;
			uint8_t file = m % 8;
			rank *= 8;
			file = 7 - file;
			return file + rank;

		}

		inline static uint8_t extract_square(const bb& my_bb) {
			return ffs(my_bb) - 1;  // ffs is 1-indexed, but chess squares are 0-indexed
		}

		inline static uint8_t extract_and_remove_square(bb& my_bb) {
			const uint8_t l = ffs(my_bb);
			my_bb &= my_bb - 1;
			return l - 1;  // ffs is 1-indexed, but chess squares are 0-indexed
		}



		static const bb BB_FULL_BOARD = 0xffffffffffffffff;
		static const bb BB_RANK8 = 0xff00000000000000;
		static const bb BB_RANK3 = 0x0000000000ff0000;
		static const bb BB_RANK4 = 0x00000000ff000000;
		static const bb BB_RANK5 = 0x000000ff00000000;
		static const bb BB_RANK6 = 0x0000ff0000000000;
		static const bb BB_RANK3N6 = BB_RANK3 | BB_RANK6;



	public: //TODO for now

		static const bitboard_set knight_moves;
		static const bitboard_set king_moves;
		static const bitboard_set bishop_moves;
		static const bitboard_set rook_moves;
		static const bitboard_set queen_moves;
		static const bitboard_set white_pawn_no_capture_moves;
		static const bitboard_set black_pawn_no_capture_moves;
		static const bitboard_set white_pawn_capture_moves;
		static const bitboard_set black_pawn_capture_moves;
	private:

	};
}
#endif /* BITBOARD_H_ */
