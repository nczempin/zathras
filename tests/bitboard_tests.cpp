#include <cassert>
#include <vector>
#include "Bitboard.h"
#include "Square.h"

using namespace Positions;

void test_ffs_functions();
void test_extract_and_remove_square();
void test_visit_bitboard();

void test_ffs_functions() {
    Square::init_squares();
    assert(Bitboard::ffs_software(0ULL) == 0);
    assert(Bitboard::ffs(0ULL) == 0);
    for (int i = 0; i < 64; ++i) {
        bb b = 1ULL << i;
        assert(Bitboard::ffs_software(b) == i + 1);
        assert(Bitboard::ffs(b) == i + 1);
    }
}

void test_extract_and_remove_square() {
    Square::init_squares();
    bb b = 0;
    Square::set_bit(b, square_t(A1));
    Square::set_bit(b, square_t(C3));
    uint8_t idx = Bitboard::extract_and_remove_square(b);
    assert(idx == 1);
    assert(b == (1ULL << C3));
}

void test_visit_bitboard() {
    Square::init_squares();
    bb b = 0;
    Square::set_bit(b, square_t(A1));
    Square::set_bit(b, square_t(B2));
    std::vector<int> visited;
    Bitboard::visit_bitboard(b, [&](square_t sq){ visited.push_back(static_cast<int>(sq)); });
    assert(visited.size() == 2);
    assert(visited[0] == 1);
    assert(visited[1] == 10);
}
