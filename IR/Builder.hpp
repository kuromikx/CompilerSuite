#pragma once
#include "pch.h"

namespace IR {
	class BasicBlock;
	class Function;
	class Types;

	class Builder {
	public:
		Builder(Function& function, Types& types);
		void SetInsertPoint(BasicBlock* block);

		// Memory
		void Store(Value* destination, Value* source);
		Value* Load(Value* left);
		
		// Math
		Value* Add(Value* left, Value* right);
		Value* Subtract(Value* left, Value* right);

		// Comparison
		Value* Less(Value* left, Value* right);
		Value* LessOrEqual(Value* left, Value* right);
		Value* Equal(Value* left, Value* right);
		Value* NotEqual(Value* left, Value* right);
		Value* Greater(Value* left, Value* right);
		Value* GreaterOrEqual(Value* left, Value* right);

		// Control Flow
		void Jump(BasicBlock* destination);
		void Branch(IR::Value* condition, BasicBlock* trueBlock, BasicBlock* falseBlock);
		void Return(Value* value = nullptr);

	protected:
		template<typename Ins>
		IR::Value* BinaryOp(IR::Value* left, IR::Value* right, IR::Type* type) {
			IR::Value* temp = function.CreateTemporary(type);
			block->Add<Ins>(temp, left, right);
			return temp;
		}

		Function& function;
		Types& types;
		BasicBlock* block;
	};
}