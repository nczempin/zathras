#include "external/catch_amalgamated.hpp"
#include "Bitboard.h"
#include "Square.h"

using namespace positions;

TEST_CASE("Bitboard - ffs functions", "[bitboard]") {
    Square::init_squares();
    REQUIRE(Bitboard::ffs_software(0ULL) == 0);
    REQUIRE(Bitboard::ffs(0ULL) == 0);
    for (int i = 0; i < 64; ++i) {
        bb b = 1ULL << i;
        REQUIRE(Bitboard::ffs_software(b) == i + 1);
        REQUIRE(Bitboard::ffs(b) == i + 1);
    }
}

TEST_CASE("Bitboard - extract and remove square", "[bitboard]") {
    Square::init_squares();
    bb b = 0;
    Square::set_bit(b, square_t(A1));
    Square::set_bit(b, square_t(C3));
    uint8_t idx = Bitboard::extract_and_remove_square(b);
    REQUIRE(idx == 1);
    REQUIRE(b == (1ULL << C3));
}

TEST_CASE("Bitboard - visit bitboard", "[bitboard]") {
    Square::init_squares();
    bb b = 0;
    Square::set_bit(b, square_t(A1));
    Square::set_bit(b, square_t(B2));
    std::vector<int> visited;
    Bitboard::visit_bitboard(b, [&](square_t sq){ visited.push_back(static_cast<int>(sq)); });
    REQUIRE(visited.size() == 2);
    REQUIRE(visited[0] == 1);
    REQUIRE(visited[1] == 10);
}
