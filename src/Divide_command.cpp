#include "Divide_command.h"
#include "Move.h"
#include "Move_state.h"
#include <iostream>
#include <iomanip>

namespace Interface {
    using namespace std;
    using namespace Moves;
    using namespace Positions;

    uint64_t Divide_command::perft(Position& pos, size_t depth) {
        if (depth == 0) {
            return 1;
        }

        Move_container move_container = mg.generate_legal_moves(pos, depth);
        
        if (depth == 1) {
            return move_container.size();
        }

        // For depth > 1, recurse
        uint64_t total_result = 0;
        auto moves = move_container.get_moves();
        
        for (size_t i = 0; i < move_container.size(); ++i) {
            Move& move = moves[i];
            Move_state ms;
            
            pos.make_move(move, ms);
            
            uint64_t perft_result = perft(pos, depth - 1);
            total_result += perft_result;
            
            pos.unmake_move(move, ms);
        }

        return total_result;
    }

    void Divide_command::execute() {
        cout << "\nDivide " << depth << " result:" << endl;

        // Generate all legal moves at root
        Move_container move_container = mg.generate_legal_moves(position, 1);
        auto moves = move_container.get_moves();
        
        uint64_t total = 0;
        
        for (size_t i = 0; i < move_container.size(); ++i) {
            Move& move = moves[i];
            Move_state ms;
            
            position.make_move(move, ms);
            
            // Calculate perft for this move
            uint64_t count = perft(position, depth - 1);
            total += count;
            
            position.unmake_move(move, ms);
            
            // Display the move and its count
            string move_str = to_string(move);
            cout << move_str << ": " << count << endl;
        }
        
        cout << "\nNodes searched: " << total << endl;
    }
}