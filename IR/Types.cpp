#include "Types.hpp"
#include "pch.h"

IR::Types::Types() {
	AddType("int1", FixedWidth(1));
	AddType("int8", FixedWidth(8));
	AddType("int16", FixedWidth(16));
	AddType("int32", FixedWidth(32));
	AddType("int64", FixedWidth(64));
}

IR::Type* IR::Types::Int1() const {
	return GetType("int1");
}

IR::Type* IR::Types::Int8() const {
	return GetType("int8");
}

IR::Type* IR::Types::Int16() const {
	return GetType("int16");
}

IR::Type* IR::Types::Int32() const {
	return GetType("int32");
}

IR::Type* IR::Types::Int64() const {
	return GetType("int64");
}

IR::Type* IR::Types::GetType(const std::string& name) const {
	return types.at(name).get();
}

IR::Type* IR::Types::AddType(const std::string& name, Type type) {
	types.emplace(name, std::make_unique<Type>(type));
	return GetType(name);
}
