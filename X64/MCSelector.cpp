#include "pch.h"
#include "MCSelector.hpp"

#include <IR/BasicBlock.hpp>

X64::MCSelector::MCSelector(X64::Allocator& allocator)
	: allocator(allocator) {
}

X64::MCBlock X64::MCSelector::Select(const IR::BasicBlock& block) {
	for (const IR::Instruction& instruction : block) {
		std::visit([&](const auto& inst) {
			Lower(inst);
		}, instruction);
	}
	return mc;
}

void X64::MCSelector::Lower(const IR::Store& store) {
	auto source = GetOperand(store.source);
	auto destination = GetOperand(store.destination);
	if (source == destination) return;
	Instruction(MC::Mov(destination, source));
}

void X64::MCSelector::Lower(const IR::Load& load) {
	auto source = GetOperand(load.source);
	auto destination = GetOperand(load.destination);
	if (source == destination) return;
	Instruction(MC::Mov({ Register::rax }, source));
	Instruction(MC::Mov(destination, { Register::rax }));
}

void X64::MCSelector::Lower(const IR::Add& add) {
	RaxCalc(MC::Add, add);
}

void X64::MCSelector::Lower(const IR::Subtract& subtract) {
	RaxCalc(MC::Sub, subtract);
}

void X64::MCSelector::Lower(const IR::Less& less) {
	CmpSetCC(MC::SetL, less);
}

void X64::MCSelector::Lower(const IR::LessOrEqual& lequal) {
	CmpSetCC(MC::SetLe, lequal);
}

void X64::MCSelector::Lower(const IR::Equal& equal) {
	CmpSetCC(MC::SetE, equal);
}

void X64::MCSelector::Lower(const IR::NotEqual& nequal) {
	CmpSetCC(MC::SetNe, nequal);
}

void X64::MCSelector::Lower(const IR::Greater& greater) {
	CmpSetCC(MC::SetG, greater);
}

void X64::MCSelector::Lower(const IR::GreaterOrEqual& gequal) {
	CmpSetCC(MC::SetGe, gequal);

}
void X64::MCSelector::Lower(const IR::Branch& bra) {
}

void X64::MCSelector::Lower(const IR::Jump& jmp) {
}

void X64::MCSelector::Lower(const IR::Return& ret) {
}

X64::Operand X64::MCSelector::GetOperand(IR::Value* value) {
	if (auto constant = dynamic_cast<IR::Constant*>(value); constant) {
		return { Immediate {std::get<IR::Constant::Integer>(constant->literal)} };
	}

	const auto Operand = [&](auto x) -> X64::Operand {
		auto allocation = allocator.GetAllocation(value);

		if (allocation.IsRegister()) {
			return { allocation.AsRegister() };
		}

		if (allocation.IsStackSlot()) {
			auto ss = allocation.AsStackSlot();
			return {
				Address{
					.base = Register::rbp,
					.offset = ss.offset,
					.index = 0,
					.scale = 1,
				}
			};
		}
	};

	if (auto temporary = dynamic_cast<IR::Temporary*>(value); temporary) {
		return Operand(temporary);
	}

	if (auto variable = dynamic_cast<IR::Variable*>(value); variable) {
		return Operand(variable);
	}

	return {};
}

void X64::MCSelector::Instruction(MC&& mc) {
	this->mc.emplace_back(std::move(mc));
}