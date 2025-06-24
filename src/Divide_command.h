#pragma once

#include "Position.h"
#include "Move_generator.h"
#include <cstddef>
#include <cstdint>

namespace Interface {
    class Divide_command {
    public:
        Divide_command(Positions::Position position, size_t depth) 
            : position(position), depth(depth) {}
        
        void execute();

    private:
        uint64_t perft(Positions::Position& pos, size_t depth);
        
        Positions::Position position;
        size_t depth;
        Moves::Move_generator mg;
    };
}