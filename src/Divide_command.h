#pragma once

#include <cstdint>
#include <memory>
#include <stack>

#include "Abstract_command.h"
#include "Position.h"
#include "Move_generator.h"
#include "Move.h"

namespace zathras::interface {
    using namespace positions;
    using namespace zathras_lib::moves;
    
    class Divide_command : public Abstract_command {
    public:
        Divide_command(const Position& position, uint8_t depth) 
            : position(position), depth(depth) {}
        
        virtual ~Divide_command();
        void execute() override;
        
    private:
        uint64_t perft(uint8_t depth);
        
        Position position;
        uint8_t depth;
        std::shared_ptr<Position> pp;
        Move_generator mg;
        std::stack<Move> s;
    };
}