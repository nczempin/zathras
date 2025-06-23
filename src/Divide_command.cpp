#include "Divide_command.h"
#include "Move.h"
#include "Move_state.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

namespace Interface {
    using namespace std;
    using namespace Moves;
    using namespace Positions;

    uint64_t Divide_command::perft(Position& pos, uint8_t depth) {
        if (depth == 0) {
            return 1;
        }

        Move_container move_container = mg.generate_pseudolegal_moves(pos, depth);
        move_container_t moves = move_container.get_moves();
        uint64_t total_result = 0;
        size_t size = move_container.size();

        for (size_t i = 0; i < size; ++i) {
            Move& move = moves[i];
            Move_state ms;
            pos.make_move(move, ms);

            // Check if the move was legal (king not in check)
            if (pos.is_in_check(!pos.white_to_move)) {
                pos.unmake_move(move, ms);
                continue;
            }

            // Recurse
            uint64_t perft_result = perft(pos, depth - 1);
            total_result += perft_result;

            pos.unmake_move(move, ms);
        }

        return total_result;
    }

    void Divide_command::execute() {
        cout << "\nDivide " << depth << " for position:" << endl;
        cout << position.print_board() << endl;

        // Generate all legal moves at root
        Move_container move_container = mg.generate_pseudolegal_moves(position, depth);
        move_container_t moves = move_container.get_moves();
        size_t move_count = move_container.size();

        // Structure to hold move results
        struct MoveResult {
            string move_str;
            uint64_t count;
        };
        vector<MoveResult> results;
        uint64_t total = 0;

        // Process each move
        for (size_t i = 0; i < move_count; ++i) {
            Move& move = moves[i];
            Move_state ms;
            position.make_move(move, ms);

            // Check if move is legal
            if (position.is_in_check(!position.white_to_move)) {
                position.unmake_move(move, ms);
                continue;
            }

            // Calculate perft for this move
            uint64_t count = (depth == 1) ? 1 : perft(position, depth - 1);
            string move_str = to_string(move);
            results.push_back({move_str, count});
            total += count;

            position.unmake_move(move, ms);
        }

        // Sort results by move string for consistent output
        sort(results.begin(), results.end(), 
             [](const MoveResult& a, const MoveResult& b) {
                 return a.move_str < b.move_str;
             });

        // Output results
        cout << "Move    Count" << endl;
        cout << "----    -----" << endl;
        for (const auto& result : results) {
            cout << left << setw(8) << result.move_str << result.count << endl;
        }
        cout << endl;
        cout << "Moves: " << results.size() << endl;
        cout << "Total: " << total << endl;
    }
}