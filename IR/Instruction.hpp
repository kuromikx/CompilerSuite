#pragma once
#include "Value.hpp"

namespace IR {
	class BasicBlock;

	template<typename Derived>
	struct Binary {
		Value* result;
		Value* left;
		Value* right;
	};

	template<typename Derived>
	struct Unary {
		Value* destination;
		Value* source;
	};

	// Memory
	struct Store : Unary<Store> {};
	struct Load : Unary<Store> {};

	// Math
	struct Add : Binary <Add> {};
	struct Subtract : Binary <Add> {};

	// Comparison
	struct Less : Binary<Less> {};
	struct LessOrEqual : Binary<LessOrEqual> {};
	struct Equal : Binary<Equal> {};
	struct NotEqual : Binary<NotEqual> {};
	struct Greater : Binary<Greater> {};
	struct GreaterOrEqual : Binary<GreaterOrEqual> {};
	
	// Control Flow
	struct Branch { Value* condition; BasicBlock* ifTrue; BasicBlock* ifFalse; };
	struct Jump { BasicBlock* destination; };
	struct Return { Value* value; };

	using Instruction = std::variant<
		Store, Load,
		Add, Subtract,
		
		Less, LessOrEqual,
		Equal, NotEqual,
		Greater, GreaterOrEqual,

		Branch, Jump, Return
	>;

	constexpr bool IsTerminator(const Instruction& instruction) {
		return
			std::holds_alternative<Branch>(instruction) ||
			std::holds_alternative<Jump>(instruction) ||
			std::holds_alternative<Return>(instruction);
	}

}