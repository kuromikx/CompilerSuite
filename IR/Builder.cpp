#include "pch.h"
#include "Builder.hpp"
#include "Function.hpp"

constexpr IR::Builder::Builder(Function& function, Types& types) 
	: function(function), types(types), block(nullptr) { }

void IR::Builder::SetInsertPoint(BasicBlock* block) {
	this->block = block;
}

void IR::Builder::Store(Value* left) {
	block->Add<IR::Store>(left);
}

IR::Value* IR::Builder::Load(Value* left) {
	IR::Value* temp = function.CreateTemporary(left->type);
	block->Add<IR::Load>(temp, left);
	return temp;
}

// Math
IR::Value* IR::Builder::Add(Value* left, Value* right) {
	return BinaryOp<IR::Add>(left, right, left->type);
}

IR::Value* IR::Builder::Subtract(Value* left, Value* right) {
	return BinaryOp<IR::Subtract>(left, right, left->type);
}

// Comparison
IR::Value* IR::Builder::Less(Value* left, Value* right) {
	return BinaryOp<IR::Less>(left, right, types.Int1());
}

IR::Value* IR::Builder::LessOrEqual(Value* left, Value* right) {
	return BinaryOp<IR::LessOrEqual>(left, right, types.Int1());
}

IR::Value* IR::Builder::Equal(Value* left, Value* right) {
	return BinaryOp<IR::Equal>(left, right, types.Int1());
}

IR::Value* IR::Builder::NotEqual(Value* left, Value* right) {
	return BinaryOp<IR::NotEqual>(left, right, types.Int1());
}

IR::Value* IR::Builder::Greater(Value* left, Value* right) {
	return BinaryOp<IR::Greater>(left, right, types.Int1());
}

IR::Value* IR::Builder::GreaterOrEqual(Value* left, Value* right) {
	return BinaryOp<IR::GreaterOrEqual>(left, right, types.Int1());
}

// Control Flow
void IR::Builder::Jump(BasicBlock* destination) {
	block->Add<IR::Jump>(destination);
}

void IR::Builder::Return(Value* value) {
	block->Add<IR::Return>(value);
}