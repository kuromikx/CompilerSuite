#pragma once
#include "Types.hpp"

#include <string>
#include <vector>
#include <span>

namespace IR {
	struct Value {
		constexpr Value(Type* type);
		virtual ~Value() = default;
		Type* type;
	};

	struct VariableMetadata {
		std::string name;
	};

	struct Variable : Value { 
		Variable(Type* type, VariableMetadata metadata);
		VariableMetadata metadata;
	};
	
	struct Constant : Value {
		using Integer = long long;
		using Float = double;
		using Literal = std::variant<Integer, Float>;

		Constant(Type* type, Literal literal);
		Literal literal;
	};

	struct Temporary : Value {
		Temporary(Type* type);
	};
}