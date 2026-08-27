#include "pch.h"
#include "Selector.hpp"

#include <IR/BasicBlock.hpp>

X64::Selector::Selector(X64::Allocator& allocator, Code& code) :
	allocator(allocator), code(code)
{ }

void X64::Selector::Select(const IR::Function& function) {
	CreateBlocks(function);

	// ...
	for (const auto& irBlock : function.GetBlocks()) {
		target = &code.GetBlock(irBlock.get());

		for (const auto& instruction : *irBlock) {
			std::visit([&](const auto& inst) {
				Lower(inst);
			}, instruction);
		}
	}
}

void X64::Selector::Lower(const IR::Store& store) {
	auto source = GetOperand(store.source);
	auto destination = GetOperand(store.destination);
	if (source == destination) return;
	Instruction(MC::Mov(destination, source));
}

void X64::Selector::Lower(const IR::Load& load) {
	auto source = GetOperand(load.source);
	auto destination = GetOperand(load.destination);
	if (source == destination) return;
	Instruction(MC::Mov({ Register::rax }, source));
	Instruction(MC::Mov(destination, { Register::rax }));
}

void X64::Selector::Lower(const IR::Add& add) {
	RaxCalc(MC::Add, add);
}

void X64::Selector::Lower(const IR::Subtract& subtract) {
	RaxCalc(MC::Sub, subtract);
}

void X64::Selector::Lower(const IR::Less& less) {
	CmpSetCC(MC::SetL, less);
}

void X64::Selector::Lower(const IR::LessOrEqual& lequal) {
	CmpSetCC(MC::SetLe, lequal);
}

void X64::Selector::Lower(const IR::Equal& equal) {
	CmpSetCC(MC::SetE, equal);
}

void X64::Selector::Lower(const IR::NotEqual& nequal) {
	CmpSetCC(MC::SetNe, nequal);
}

void X64::Selector::Lower(const IR::Greater& greater) {
	CmpSetCC(MC::SetG, greater);
}

void X64::Selector::Lower(const IR::GreaterOrEqual& gequal) {
	CmpSetCC(MC::SetGe, gequal);

}

void X64::Selector::CreateBlocks(const IR::Function& function) {
	const auto& blocks = function.GetBlocks();
	code.CreateBlock(blocks[0].get(), Label{ std::string(function.GetName()) });

	std::size_t nextLabelId = 0;

	for (const auto& block : blocks) {
		code.CreateBlock(block.get(), { nextLabelId++ });
	}
}

void X64::Selector::Lower(const IR::Branch& bra) {
	const auto condition = GetOperand(bra.condition);
	Instruction(MC::Test(condition, condition));
	Instruction(MC::Jnz({ GetBlockLabel(bra.trueBlock) }));
	Instruction(MC::Jz({ GetBlockLabel(bra.falseBlock) }));
}

void X64::Selector::Lower(const IR::Jump& jmp) {
	Instruction(MC::Jmp({ GetBlockLabel(jmp.destination) }));
}

void X64::Selector::Lower(const IR::Return& ret) {
	if (ret.value) {
		Instruction(MC::Mov({ Register::rax }, GetOperand(ret.value)));
	}
	Instruction(MC::Ret());
}

X64::Operand X64::Selector::GetOperand(IR::Value* value) {
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

X64::Label X64::Selector::GetBlockLabel(IR::BasicBlock* block) {
	return code.GetLabel(block);
}

void X64::Selector::Instruction(MC&& mc) {
	target->emplace_back(std::move(mc));
}
