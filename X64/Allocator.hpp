#pragma once
#include <array>
#include <IR/InterferenceGraph.hpp>
#include "Regs.hpp"

namespace X64 {
    class Allocator {
    public:
        struct StackSlot { std::size_t offset; };
        
        class Allocation {
        public:
            Allocation(Register reg)
                : value(reg) {
            }

            Allocation(StackSlot slot)
                : value(slot) {
            }
            const Register& AsRegister() const;
            const StackSlot& AsStackSlot() const;
            bool IsRegister() const;
            bool IsStackSlot() const;
            void Print() const;

        private:
            std::variant<Register, StackSlot> value;
        };

        using Allocations = std::unordered_map<IR::Value*, Allocation>;

        Allocations Allocate(const IR::InterferenceGraph& graph);

    private:
        static constexpr std::array<Register, 13> registers =
        {
            Register::rbx,
            Register::rcx,
            Register::rdx,
            Register::rsi,
            Register::rdi,
            Register::r8,
            Register::r9,
            Register::r10,
            Register::r11,
            Register::r12,
            Register::r13,
            Register::r14,
            Register::r15
        };
    };
}