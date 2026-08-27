#include "Operand.hpp"
#include "pch.h"

bool X64::Operand::IsRegister() const {
	return std::holds_alternative<X64::Register>(*this);
}

bool X64::Operand::IsMemoryAddress() const {
	return std::holds_alternative<X64::Address>(*this);
}

bool X64::Operand::IsImmediate() const {
	return std::holds_alternative<X64::Immediate>(*this);
}

bool X64::Operand::IsRax() const {
	return IsRegister() && AsRegister() == Register::rax;
}

bool X64::Operand::IsLabel() const {
	return std::holds_alternative<X64::Label>(*this);
}

const X64::Register& X64::Operand::AsRegister() const {
	return std::get<X64::Register>(*this);
}

const X64::Address& X64::Operand::AsMemoryAddress() const {
	return std::get<X64::Address>(*this);
}

const X64::Immediate& X64::Operand::AsImmediate() const {
	return std::get<X64::Immediate>(*this);
}

const X64::Label& X64::Operand::AsLabel() const {
	return std::get<X64::Label>(*this);
}

bool X64::Operand::operator==(const Operand& rhs) const {
	if (IsRegister() && rhs.IsRegister()) return AsRegister() == rhs.AsRegister();
	if (IsImmediate() && rhs.IsImmediate()) return AsImmediate() == rhs.AsImmediate();
	if (IsMemoryAddress() && rhs.IsMemoryAddress()) return AsMemoryAddress() == rhs.AsMemoryAddress();
	if (IsLabel() && rhs.IsLabel()) return AsLabel() == rhs.AsLabel();
	return false;
}


std::string X64::Label::ToString() const {
	if (std::holds_alternative<std::size_t>(id)) {
		return ".L" + std::to_string(std::get<std::size_t>(id));
	}
	if (std::holds_alternative<std::string>(id)) {
		return std::get<std::string>(id);
	}
	throw;
}