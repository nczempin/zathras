#include <iostream>
#include "Square.h"

using namespace positions;

void test_ffs_functions();
void test_extract_and_remove_square();
void test_visit_bitboard();
void test_square_functions();
void test_bitboard_ffs();

int main() {
    Square::init_squares();
    test_ffs_functions();
    test_extract_and_remove_square();
    test_visit_bitboard();
    test_square_functions();
    test_bitboard_ffs();
    std::cout << "All tests passed\n";
    return 0;
}
