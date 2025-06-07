#include "Square.h"
#include "typedefs.h"
#include <cassert>
#include <string>

void test_square_functions() {
    using namespace positions;
    assert(Square::mailbox_index_to_square(0) == std::string("a1"));
    assert(Square::mailbox_index_to_square(63) == std::string("h8"));
}
