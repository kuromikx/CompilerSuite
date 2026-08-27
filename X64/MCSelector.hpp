#pragma once
#include <IR/Instruction.hpp>

#include "MCBlock.hpp"
#include "Allocator.hpp"

namespace X64 {
	class MCSelector {
	public:
		MCSelector(X64::Allocator& allocator);
		MCBlock Select(const IR::BasicBlock& block);
	private:
		void Lower(const IR::Store&);
		void Lower(const IR::Load&);
		void Lower(const IR::Add&);
		void Lower(const IR::Subtract&);
		void Lower(const IR::Less&);
		void Lower(const IR::LessOrEqual&);
		void Lower(const IR::Equal&);
		void Lower(const IR::NotEqual&);
		void Lower(const IR::Greater&);
		void Lower(const IR::GreaterOrEqual&);
		void Lower(const IR::Branch&);
		void Lower(const IR::Jump&);
		void Lower(const IR::Return&);
		
		Operand GetOperand(IR::Value* value);
		void Instruction(MC&& mc);
		
		template<typename Binary>
		void CmpSetCC(auto SetCC, Binary bin) {
			Instruction(MC::Cmp(
				GetOperand(bin.left),
				GetOperand(bin.right)
			));

			Instruction(SetCC(
				GetOperand(bin.result)
			));
		}

		template<typename Binary>
		void RaxCalc(auto Fn, Binary bin) {
			Instruction(MC::Mov({ Register::rax }, GetOperand(bin.left)));
			Instruction(Fn({ Register::rax }, GetOperand(bin.right)));
			Instruction(MC::Mov(GetOperand(bin.result), { Register::rax }));
		}

		MCBlock mc;
		Allocator& allocator;
	};
}
