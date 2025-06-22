#include <iostream>
#include <climits>
#include "zathras_lib/src/Position.h"
#include "zathras_lib/src/Move_generator.h"

using namespace std;
using namespace Positions;
using namespace Moves;

uint64_t perft(Position& pos, Move_generator& mg, int depth) {
    if (depth == 0) return 1;
    
    Move_container moves = mg.generate_legal_moves(pos, depth);
    if (depth == 1) return moves.size();
    
    uint64_t nodes = 0;
    auto move_list = moves.get_moves();
    
    for (int i = 0; i < moves.size(); i++) {
        Move move = move_list[i];
        Move_state ms;
        pos.make_move(move, ms);
        nodes += perft(pos, mg, depth - 1);
        pos.unmake_move(move, ms);
    }
    
    return nodes;
}

int main() {
    Position pos = Position::create_start_position();
    Move_generator mg;
    
    uint64_t result = perft(pos, mg, 3);
    cout << "Perft 3 result: " << result << endl;
    
    return result == 8902 ? 0 : 1;
}
