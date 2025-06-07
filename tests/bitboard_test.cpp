#include <iostream>
#include <cassert>
#include "Bitboard.h"
#include "typedefs.h"

using namespace positions;

int main() {
    std::cout << "Running Bitboard tests..." << std::endl;
    
    // Test first set bit function
    assert(Bitboard::ffs(1ULL) == 1);
    assert(Bitboard::ffs(0x10ULL) == 5);
    assert(Bitboard::ffs(0x8000000000000000ULL) == 64);
    std::cout << "✓ Bitboard::ffs tests passed" << std::endl;
    
    // Test extract_square function
    bb test_bb = 0x10ULL; // bit 4 set
    assert(Bitboard::extract_square(test_bb) == 4);
    std::cout << "✓ Bitboard::extract_square tests passed" << std::endl;
    
    // Test extract_and_remove_square function
    bb test_bb2 = 0x11ULL; // bits 0 and 4 set
    uint8_t square = Bitboard::extract_and_remove_square(test_bb2);
    assert(square == 0); // should extract the first set bit
    assert(test_bb2 == 0x10ULL); // bit 0 should be removed
    std::cout << "✓ Bitboard::extract_and_remove_square tests passed" << std::endl;
    
    std::cout << "All Bitboard tests passed!" << std::endl;
    return 0;
}