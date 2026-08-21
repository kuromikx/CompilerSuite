#include "pch.h"

constexpr IR::Value::Value(Type* type) : type(type)
{}

 IR::Variable::Variable(Type* type, VariableMetadata metadata) :
	Value(type),
	metadata(metadata)
{}

IR::Constant::Constant(Type* type, Literal literal) : Value(type), literal(literal)
{}

IR::Temporary::Temporary(Type* type) : Value(type)
{}

