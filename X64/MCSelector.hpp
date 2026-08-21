#pragma once
#include <IR/Instruction.hpp>

#include "MCBlock.hpp"

namespace X64 {
	class MCSelector {
	public:
		MCBlock Lower(const IR::BasicBlock& block);
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

		MCBlock mc;
	};
}
