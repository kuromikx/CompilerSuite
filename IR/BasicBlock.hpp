#pragma once
#include <vector>
#include "Instruction.hpp"

namespace IR {
	class BasicBlock {
	private:
		using Instructions = std::vector<Instruction>;
		using Iterator = Instructions::const_iterator;
		using ReverseIterator = Instructions::const_reverse_iterator;

	public:
		// Adds an instruction to this basic block.
		template<typename T, typename... Args>
		void Add(Args&&... args) {
			instructions.emplace_back(Instruction{ T{std::forward<Args>(args)...} });
		}

		Instruction GetTerminator() const;
		Iterator begin() const;
		Iterator end() const;
		ReverseIterator rbegin() const;
		ReverseIterator rend() const;

	private:
		Instructions instructions;
	};


}