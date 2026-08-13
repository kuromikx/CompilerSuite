#include "pch.h"

constexpr IR::Value::Value(Type* type) : type(type)
{}

constexpr IR::Variable::Variable(Type* type, VariableMetadata metadata) :
	Value(type),
	metadata(metadata)
{}

constexpr IR::Constant::Constant(Type* type, Literal literal) : Value(type), literal(literal)
{}

constexpr IR::Temporary::Temporary(Type* type) : Value(type)
{}

