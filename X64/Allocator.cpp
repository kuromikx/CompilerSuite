#include "pch.h"
#include "Allocator.hpp"
#include <iostream>

X64::Allocator::Allocations X64::Allocator::Allocate(const IR::InterferenceGraph& graph) {
    constexpr std::size_t registerCount = registers.size();

    // Values which haven't been removed from the graph yet.
    std::unordered_set<IR::Value*> remaining;

    for (const auto& [value, neighbours] : graph.GetValues()) {
        remaining.insert(value);
    }

    // Calculate the degree of a node considering only nodes which
    // are still in the graph.
    const auto degree = [&](IR::Value* value) {
        std::size_t result = 0;

        for (auto* neighbour : graph.GetNeighbours(value)) {
            if (remaining.contains(neighbour)) {
                ++result;
            }
        }

        return result;
    };

    // Nodes removed during the simplify phase.
    std::vector<IR::Value*> stack;

    // Nodes which we decided may need spilling.
    std::unordered_set<IR::Value*> potentialSpills;

    while (!remaining.empty()) {
        // Find something we can simplify.
        auto it = std::find_if(
            remaining.begin(),
            remaining.end(),
            [&](IR::Value* value) {
                return degree(value) < registerCount;
            }
        );

        if (it == remaining.end()) {
            // Everything currently has degree >= K.
            //
            // Pick a spill candidate. A better allocator would use a
            // spill cost here, but choosing the highest-degree node is
            // a perfectly reasonable first implementation.
            it = std::max_element(
                remaining.begin(),
                remaining.end(),
                [&](IR::Value* a, IR::Value* b) {
                    return degree(a) < degree(b);
                }
            );

            potentialSpills.insert(*it);
        }

        stack.push_back(*it);
        remaining.erase(it);
    }

    Allocations allocation;

    // Stack slots are allocated in bytes.
    std::size_t nextStackSlot = 0;

    while (!stack.empty()) {
        IR::Value* value = stack.back();
        stack.pop_back();

        std::unordered_set<Register> usedRegisters;

        for (auto* neighbour : graph.GetNeighbours(value)) {
            auto it = allocation.find(neighbour);

            if (it == allocation.end()) {
                continue;
            }

            if (it->second.IsRegister()) {
                usedRegisters.insert(
                    it->second.AsRegister()
                );
            }
        }

        auto registerIt = std::find_if(
            registers.begin(),
            registers.end(),
            [&](Register reg) {
                return !usedRegisters.contains(reg);
            }
        );

        if (registerIt != registers.end()) {
            allocation.insert_or_assign(
                value,
                X64::Register{ *registerIt }
            );
        }
        else {
            allocation.insert_or_assign(
                value,
                StackSlot{ .offset = nextStackSlot }
            );

            nextStackSlot += 8;
        }
    }

    return allocation;
}

const X64::Register& X64::Allocator::Allocation::AsRegister() const {
    return std::get<Register>(value);
}


const X64::Allocator::StackSlot& X64::Allocator::Allocation::AsStackSlot() const {
    return std::get<StackSlot>(value);
}

bool X64::Allocator::Allocation::IsRegister() const {
    return std::holds_alternative<Register>(value);
}

bool X64::Allocator::Allocation::IsStackSlot() const {
    return std::holds_alternative<StackSlot>(value);
}

void X64::Allocator::Allocation::Print() const {
    if (IsRegister()) {
        std::cout << "Register: " << ToString(AsRegister()) << '\n';
    }
    else if (IsStackSlot()) {
        std::cout << "Slot: " << AsStackSlot().offset << '\n';
    }
}
