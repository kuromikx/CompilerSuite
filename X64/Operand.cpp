#include "Operand.hpp"
#include "pch.h"

constexpr bool X64::Operand::IsRegister() const {
	return std::holds_alternative<X64::Register>(*this);
}

constexpr bool X64::Operand::IsMemoryAddress() const {
	return std::holds_alternative<X64::Address>(*this);
}

constexpr bool X64::Operand::IsImmediate() const {
	return std::holds_alternative<X64::Immediate>(*this);
}

constexpr bool X64::Operand::IsRax() const {
	return IsRegister() && AsRegister() == Register::rax;
}

constexpr const X64::Register& X64::Operand::AsRegister() const {
	return std::get<X64::Register>(*this);
}

constexpr const X64::Address& X64::Operand::AsMemoryAddress() const {
	return std::get<X64::Address>(*this);
}

constexpr const X64::Immediate& X64::Operand::AsImmediate() const {
	return std::get<X64::Immediate>(*this);
}