#ifndef DIVIDE_COMMAND_H_
#define DIVIDE_COMMAND_H_

#include "Abstract_command.h"
#include "Position.h"
#include "Move_generator.h"
#include <memory>

namespace Interface {
    class Divide_command : public Abstract_command {
    public:
        Divide_command(Positions::Position position, size_t depth) 
            : position(position), depth(depth) {}
        virtual ~Divide_command() = default;
        void execute() override;

    private:
        uint64_t perft(Positions::Position& pos, uint8_t depth);
        
        Positions::Position position;
        size_t depth;
        Moves::Move_generator mg;
    };
}

#endif /* DIVIDE_COMMAND_H_ */