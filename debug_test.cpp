#include <iostream>
#include <cassert>
#include "zathras_lib/src/Bitboard.h"
#include "zathras_lib/src/Square.h"

using namespace positions;

int main() {
    std::cout << "=== Debugging Bitboard Functions ===" << std::endl;
    
    Square::init_squares();
    
    // Check the actual square values
    std::cout << "Square values:" << std::endl;
    std::cout << "A1 = " << static_cast<int>(A1) << std::endl;
    std::cout << "B1 = " << static_cast<int>(B1) << std::endl;
    std::cout << "C3 = " << static_cast<int>(C3) << std::endl;
    std::cout << "B2 = " << static_cast<int>(B2) << std::endl;
    
    // Test ffs function behavior
    std::cout << "\n=== Testing ffs functions ===" << std::endl;
    std::cout << "ffs(1ULL) = " << static_cast<int>(Bitboard::ffs(1ULL)) << std::endl;
    std::cout << "ffs(2ULL) = " << static_cast<int>(Bitboard::ffs(2ULL)) << std::endl;
    std::cout << "ffs(4ULL) = " << static_cast<int>(Bitboard::ffs(4ULL)) << std::endl;
    std::cout << "ffs(8ULL) = " << static_cast<int>(Bitboard::ffs(8ULL)) << std::endl;
    
    // Test with actual chess square bitboards
    bb test_bb = 0;
    Square::set_bit(test_bb, square_t(A1));
    std::cout << "\nWith A1 set:" << std::endl;
    std::cout << "bitboard = 0x" << std::hex << test_bb << std::dec << std::endl;
    std::cout << "ffs(bitboard) = " << static_cast<int>(Bitboard::ffs(test_bb)) << std::endl;
    std::cout << "extract_square(bitboard) = " << static_cast<int>(Bitboard::extract_square(test_bb)) << std::endl;
    
    // Test extract_and_remove_square 
    std::cout << "\n=== Testing extract_and_remove_square ===" << std::endl;
    bb b = 0;
    Square::set_bit(b, square_t(A1));
    Square::set_bit(b, square_t(C3));
    std::cout << "Before: bitboard = 0x" << std::hex << b << std::dec << std::endl;
    uint8_t idx = Bitboard::extract_and_remove_square(b);
    std::cout << "After: bitboard = 0x" << std::hex << b << std::dec << std::endl;
    std::cout << "Extracted index = " << static_cast<int>(idx) << std::endl;
    std::cout << "Expected index = " << static_cast<int>(A1) << std::endl;
    std::cout << "Test expects: idx == 1, but A1 = " << static_cast<int>(A1) << std::endl;
    
    // Test visit_bitboard
    std::cout << "\n=== Testing visit_bitboard ===" << std::endl;
    bb visit_b = 0;
    Square::set_bit(visit_b, square_t(A1));
    Square::set_bit(visit_b, square_t(B2));
    std::cout << "Bitboard with A1 and B2: 0x" << std::hex << visit_b << std::dec << std::endl;
    
    std::vector<int> visited;
    Bitboard::visit_bitboard(visit_b, [&](square_t sq){ 
        visited.push_back(static_cast<int>(sq)); 
        std::cout << "Visited square: " << static_cast<int>(sq) << std::endl;
    });
    
    std::cout << "A1 = " << static_cast<int>(A1) << ", B2 = " << static_cast<int>(B2) << std::endl;
    std::cout << "Test expects visited[0] == 1, but got " << visited[0] << std::endl;
    std::cout << "Test expects visited[1] == 10, but got " << visited[1] << std::endl;
    
    return 0;
}