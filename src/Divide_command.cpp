#include "Divide_command.h"

#include <iostream>
#include <string>
#include <cassert>

namespace zathras::interface {
    using namespace std;
    using namespace zathras_lib::moves;

    Divide_command::~Divide_command() {
    }

    uint64_t Divide_command::perft(uint8_t depth) {
        if (depth == 0) {
            return 1;
        }
        
        Move_container move_container = mg.generate_pseudolegal_moves(*pp, depth);
        move_container_t moves = move_container.get_moves();
        uint64_t total_result = 0;
        size_t size = move_container.size();

        for (size_t i = 0; i < size; ++i) {
            Move& move = moves[i];
            Move_state ms;
            
            // Check if there's actually a piece to move
            square_t from = get_from(move);
            piece_t moving_piece = pp->get_piece_on(from);
            if (moving_piece == 0) {
                cout << "Warning: Trying to move from empty square " << from << endl;
                continue;
            }
            
            pp->make_move(move, ms);
            s.push(move);

            if (pp->is_in_check(!pp->white_to_move, &s)) {
                pp->unmake_move(move, ms);
                s.pop();
                continue;
            }

            // The move was legal
            if (depth == 1) {
                ++total_result;
            } else {
                uint64_t perft_result = perft(depth - 1);
                total_result += perft_result;
            }
            
            pp->unmake_move(move, ms);
            s.pop();
        }

        return total_result;
    }

    void Divide_command::execute() {
        cout << "Divide " << to_string(depth) << " for current position:" << endl;
        cout << position.print_board();
        
        pp = make_shared<Position>(position);
        Move_container move_container = mg.generate_pseudolegal_moves(position, depth);
        size_t move_count = move_container.size();
        uint64_t total_result = 0;

        if (depth == 0) {
            cout << "Depth 0: " << move_count << " moves" << endl;
            return;
        }

        move_container_t moves = move_container.get_moves();

        for (size_t i = 0; i < move_count; ++i) {
            Move& move = moves[i];
            Move_state ms;
            
            // Check if there's actually a piece to move
            square_t from = get_from(move);
            piece_t moving_piece = pp->get_piece_on(from);
            if (moving_piece == 0) {
                cout << "Warning: Generated move from empty square " << from << endl;
                continue;
            }
            
            pp->make_move(move, ms);
            
            if (pp->is_in_check(!pp->white_to_move)) {
                pp->unmake_move(move, ms);
                continue;
            }

            uint64_t perft_result = perft(static_cast<uint8_t>(depth - 1));
            string move_str = to_string(move);
            cout << move_str << ": " << perft_result << endl;
            total_result += perft_result;
            pp->unmake_move(move, ms);
        }

        cout << endl << "Total: " << total_result << endl;
    }
}