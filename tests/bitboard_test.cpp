#include "external/catch_amalgamated.hpp"
#include "Bitboard.h"
#include "typedefs.h"

using namespace Positions;

TEST_CASE("Bitboard ffs finds first set bit", "[bitboard]") {
    REQUIRE(Bitboard::ffs(1ULL) == 1);
    REQUIRE(Bitboard::ffs(0x10ULL) == 5);
}
