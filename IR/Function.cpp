#include "pch.h"
#include "Function.hpp"
#include "Value.hpp"

IR::BasicBlock* IR::Function::CreateBlock() {
	blocks.emplace_back(new BasicBlock());
	return blocks.back().get();
}

IR::Value* IR::Function::CreateVariable(Type* type, VariableMetadata metadata) {
	values.emplace_back(new Variable(type, metadata));
	return values.back().get();
}

IR::Value* IR::Function::CreateConstant(IR::Type* type, Constant::Literal literal) {
	values.emplace_back(new Constant(type, literal));
	return values.back().get();
}

IR::Value* IR::Function::CreateTemporary(Type* type) {
	values.emplace_back(new Temporary(type));
	return values.back().get();
}

std::span<const std::unique_ptr<IR::BasicBlock>> IR::Function::GetBlocks() const {
	return blocks;
}
