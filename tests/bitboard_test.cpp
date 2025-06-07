#include <iostream>
#include <cassert>
#include "Bitboard.h"
#include "typedefs.h"

using namespace Positions;

void test_bitboard_ffs() {
    std::cout << "Testing Bitboard::ffs..." << std::endl;
    
    // Test first set bit
    assert(Bitboard::ffs(1ULL) == 1);
    assert(Bitboard::ffs(0x10ULL) == 5);
    
    std::cout << "Bitboard::ffs tests passed!" << std::endl;
}

int main() {
    test_bitboard_ffs();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
