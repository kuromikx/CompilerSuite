#include "pch.h"
#include "MCSelector.hpp"

#include <IR/BasicBlock.hpp>

X64::MCBlock X64::MCSelector::Lower(const IR::BasicBlock& block) {
	for (const IR::Instruction& instruction : block) {
		std::visit([&](const auto& inst) {
			Lower(inst);
			}, instruction);
	}
	return mc;
}

void X64::MCSelector::Lower(const IR::Store& store) {
	Instruction(MC::Mov(GetOperand(store.destination), GetOperand(store.source)));
}

void X64::MCSelector::Lower(const IR::Load& load) {
	Instruction(MC::Mov({ Register::rax }, GetOperand(load.source)));
	Instruction(MC::Mov(GetOperand(load.destination), { Register::rax }));
}

void X64::MCSelector::Lower(const IR::Add& add) {
	Instruction(MC::Mov({ Register::rax }, GetOperand(add.left)));
	Instruction(MC::Add({ Register::rax }, GetOperand(add.right)));
	Instruction(MC::Mov(GetOperand(add.result), { Register::rax }));
}

void X64::MCSelector::Lower(const IR::Subtract& subtract) {
	Instruction(MC::Mov({ Register::rax }, GetOperand(subtract.left)));
	Instruction(MC::Sub({ Register::rax }, GetOperand(subtract.right)));
	Instruction(MC::Mov(GetOperand(subtract.result), { Register::rax }));
}

void X64::MCSelector::Lower(const IR::Less& less) {
}

void X64::MCSelector::Lower(const IR::LessOrEqual& lequal) {
}

void X64::MCSelector::Lower(const IR::Equal& equal) {
}

void X64::MCSelector::Lower(const IR::NotEqual& nequal) {
}

void X64::MCSelector::Lower(const IR::Greater& greater) {
}

void X64::MCSelector::Lower(const IR::GreaterOrEqual& gequal) {
}

void X64::MCSelector::Lower(const IR::Branch& bra) {
}

void X64::MCSelector::Lower(const IR::Jump& jmp) {
}

void X64::MCSelector::Lower(const IR::Return& ret) {
}

X64::Operand X64::MCSelector::GetOperand(IR::Value* value) {
	return {};
}

void X64::MCSelector::Instruction(MC&& mc) {
	this->mc.emplace_back(std::move(mc));
}
