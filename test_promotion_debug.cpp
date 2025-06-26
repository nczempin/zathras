#include <iostream>
#include <string>
#include "Position.h"
#include "Move_generator.h"
#include "Move.h"

using namespace std;
using namespace Positions;
using namespace Moves;

int main() {
    // Test position: white pawn about to promote
    Position pos = Position::create_position("4k3/P7/8/8/8/8/8/4K3 w - - 0 1");
    Move_generator mg;
    
    cout << "Initial position:" << endl;
    pos.print(cout);
    
    // Generate moves
    Move_container moves = mg.generate_legal_moves(pos, 1);
    cout << "\nWhite has " << moves.size() << " moves:" << endl;
    
    auto move_list = moves.get_moves();
    for (size_t i = 0; i < moves.size(); ++i) {
        Move& move = move_list[i];
        string move_str = to_string(move);
        
        // Make the move
        Move_state ms;
        pos.make_move(move, ms);
        
        // Check what piece is on a8 after promotion
        if (move_str.find("a7a8") != string::npos) {
            piece_t piece_on_a8 = pos.get_piece_on(56); // a8 = square 56
            cout << "\n" << move_str << " - Piece on a8: " << (int)piece_on_a8;
            
            // Generate black's moves
            Move_container black_moves = mg.generate_legal_moves(pos, 1);
            cout << " - Black has " << black_moves.size() << " moves" << endl;
        }
        
        // Unmake the move
        pos.unmake_move(move, ms);
    }
    
    return 0;
}