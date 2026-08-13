#pragma once
#include <vector>
#include "Instruction.hpp"

namespace IR {

    /**
     * Represents a basic block in the intermediate representation.
     */
    class BasicBlock {
    private:
        using Instructions = std::vector<Instruction>;
        using Iterator = Instructions::const_iterator;

    public:
        // Adds an instruction to this basic block.
        template<typename Ins, typename... Args>
        void Add(Args&&... args) {
            instructions.emplace_back(Instruction{ Ins{std::forward<Args>(args)...} });
        }

        Iterator begin() const;
        Iterator end() const;

    private:
        Instructions instructions;
    };
}