#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>

#include "zathras_lib.h"
#include "Bitboard.h"
#include "Position.h"
#include "Move_generator.h"
#include "Move_state.h"

namespace positions = Positions;
using namespace Positions;
using namespace Moves;

static uint64_t run_perft(Position& pos, Move_generator& mg, size_t depth) {
    if (depth == 0) {
        return 1;
    }
    Move_container mc = mg.generate_legal_moves(pos);
    if (depth == 1) {
        return mc.size();
    }
    uint64_t total = 0;
    auto moves = mc.get_moves();
    for (size_t i = 0; i < mc.size(); ++i) {
        Move& m = moves[i];
        Move_state ms;
        pos.make_move(m, ms);
        total += run_perft(pos, mg, depth - 1);
        pos.unmake_move(m, ms);
    }
    return total;
}

static int g_failures = 0;

static void verify_perft(const std::string& name, const std::string& fen, size_t depth, uint64_t expected) {
    Position pos = Position::create_position(fen);
    Move_generator mg;
    uint64_t actual = run_perft(pos, mg, depth);
    std::cout << "[TEST] " << name << " perft(" << depth << ") = " << actual
              << " (expected " << expected << ")" << std::endl;
    if (actual != expected) {
        std::cerr << "[FAIL] " << name << " perft(" << depth << ") got " << actual
                  << ", expected " << expected << std::endl;
        ++g_failures;
    }
}

int main() {
    Zathras_lib::init();

    // Test 1: Start position
    verify_perft("Startpos", "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 1, 20);
    verify_perft("Startpos", "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 2, 400);
    verify_perft("Startpos", "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 3, 8902);

    // Test 2: Kiwipete (castling, ep, promotions)
    verify_perft("Kiwipete", "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 1, 48);
    verify_perft("Kiwipete", "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 2, 2039);

    // Test 3: CPW Position 3 (en-passant discovered check pin)
    verify_perft("CPW Pos 3", "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 1, 14);
    verify_perft("CPW Pos 3", "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 2, 191);
    verify_perft("CPW Pos 3", "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 3, 2812);

    // Test 4: CPW Position 4 (promotions & castling rights)
    verify_perft("CPW Pos 4", "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 1, 6);
    verify_perft("CPW Pos 4", "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 2, 264);

    // Test 5: TalkChess Position 5 (promotion at ply 1)
    verify_perft("TalkChess Pos 5", "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 1, 44);
    verify_perft("TalkChess Pos 5", "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 2, 1486);

    if (g_failures != 0) {
        std::cerr << g_failures << " unit test(s) failed!" << std::endl;
        return 1;
    }
    std::cout << "All unit tests passed!" << std::endl;
    return 0;
}
