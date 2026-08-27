#include "pch.h"
#include "MC.hpp"

#include <sstream>
#include <variant>

namespace X64 {

	std::string ToString(const X64::Immediate& immediate) {
		return std::to_string(immediate.value);
	}

	std::string ToString(const X64::Address& address) {
		std::ostringstream result;

		result << '[';

		bool hasTerm = false;

		result << ToString(address.base);
		hasTerm = true;

		if (address.index != 0) {
			if (hasTerm) {
				result << " + ";
			}

			result << address.index;

			if (address.scale != 1) {
				result << " * " << address.scale;
			}

			hasTerm = true;
		}

		if (address.offset != 0) {
			if (hasTerm && address.offset > 0) {
				result << " + ";
			}

			if (address.offset < 0) {
				result << " - " << -address.offset;
			}
			else {
				result << address.offset;
			}
		}

		result << ']';

		return result.str();
	}

	static std::string ToString(const X64::Label& label) {
		return label.ToString();
	}

}

std::string X64::MC::ToString(const Operand& operand) {
	return std::visit(
		[](const auto& value) -> std::string {
			return X64::ToString(value);
		},
		operand
	);
}

// Storage
X64::MC X64::MC::Mov(Operand dst, Operand src) {
	return { Opcode::Mov, dst, src };
}

X64::MC X64::MC::MovZx(Operand dst, Operand src) {
	return { Opcode::MovZx, dst, src };
}


// Math
X64::MC X64::MC::Add(Operand dst, Operand src) {
	return { Opcode::Add, dst, src };
}

X64::MC X64::MC::Sub(Operand dst, Operand src) {
	return { Opcode::Sub, dst, src };
}

X64::MC X64::MC::Inc(Operand dst) {
	return { Opcode::Inc, dst };
}

X64::MC X64::MC::Dec(Operand dst) {
	return { Opcode::Dec, dst };
}


// Logic
X64::MC X64::MC::And(Operand dst, Operand src) {
	return { Opcode::And, dst, src };
}

X64::MC X64::MC::Or(Operand dst, Operand src) {
	return { Opcode::Or, dst, src };
}

X64::MC X64::MC::Xor(Operand dst, Operand src) {
	return { Opcode::Xor, dst, src };
}


// Comparisons
X64::MC X64::MC::Cmp(Operand left, Operand right) {
	return { Opcode::Cmp, left, right };
}

X64::MC X64::MC::Test(Operand left, Operand right) {
	return { Opcode::Test, left, right };
}

X64::MC X64::MC::SetL(Operand dst) {
	return { Opcode::SetL, dst };
}

X64::MC X64::MC::SetLe(Operand dst) {
	return { Opcode::SetLe, dst };
}

X64::MC X64::MC::SetE(Operand dst) {
	return { Opcode::SetE, dst };
}

X64::MC X64::MC::SetNe(Operand dst) {
	return { Opcode::SetNE, dst };
}

X64::MC X64::MC::SetG(Operand dst) {
	return { Opcode::SetG, dst };
}

X64::MC X64::MC::SetGe(Operand dst) {
	return { Opcode::SetGe, dst };
}

X64::MC X64::MC::SetZ(Operand dst) {
	return { Opcode::SetZ, dst };
}

X64::MC X64::MC::SetNZ(Operand dst) {
	return { Opcode::SetNZ, dst };
}


// Branching
X64::MC X64::MC::Jmp(const Operand dst) {
	return { Opcode::Jmp, dst };
}

X64::MC X64::MC::Jz(const Operand dst) {
	return { Opcode::Jz, dst };
}

X64::MC X64::MC::Jnz(const Operand dst) {
	return { Opcode::Jnz, dst };
}

X64::MC X64::MC::Jl(const Operand dst) {
	return { Opcode::Jl, dst };
}

X64::MC X64::MC::Jle(const Operand dst) {
	return { Opcode::Jle, dst };
}

X64::MC X64::MC::Je(const Operand dst) {
	return { Opcode::Je, dst };
}

X64::MC X64::MC::Jne(const Operand dst) {
	return { Opcode::Jne, dst };
}

X64::MC X64::MC::Jg(const Operand dst) {
	return { Opcode::Jg, dst };
}

X64::MC X64::MC::Jge(const Operand dst) {
	return { Opcode::Jge, dst };
}


X64::MC X64::MC::Ret() {
	return { Opcode::Ret };
}

X64::MC X64::MC::Nop() {
	return { Opcode::Nop };
}


// Helpers
bool X64::MC::IsConditionalJump() const noexcept {
	switch (opcode) {
	case Opcode::Jl:
	case Opcode::Jle:
	case Opcode::Je:
	case Opcode::Jne:
	case Opcode::Jg:
	case Opcode::Jge:
	case Opcode::Jz:
	case Opcode::Jnz:
		return true;
	}

	return false;
}

bool X64::MC::IsSetCC() const noexcept {
	switch (opcode) {
	case Opcode::SetL:
	case Opcode::SetLe:
	case Opcode::SetG:
	case Opcode::SetGe:
	case Opcode::SetE:
	case Opcode::SetNE:
	case Opcode::SetZ:
	case Opcode::SetNZ:
		return true;
	}

	return false;
}

X64::MC X64::MC::InvertJumpCondition() const noexcept {
	switch (opcode) {
	case Opcode::Jl:
		return Jge(operands.Destination());

	case Opcode::Jle:
		return Jl(operands.Destination());

	case Opcode::Je:
		return Jne(operands.Destination());

	case Opcode::Jne:
		return Je(operands.Destination());

	case Opcode::Jg:
		return Jle(operands.Destination());

	case Opcode::Jge:
		return Jl(operands.Destination());

	case Opcode::Jz:
		return Jnz(operands.Destination());

	case Opcode::Jnz:
		return Jz(operands.Destination());
	}

	return *this;
}


// Opcode -> NASM mnemonic
const char* X64::MC::ToString(Opcode opcode) {
	using enum Opcode;

	switch (opcode) {
	case Mov:   return "mov";
	case MovZx: return "movzx";

	case Push: return "push";
	case Pop:  return "pop";

	case Add: return "add";
	case Sub: return "sub";
	case Inc: return "inc";
	case Dec: return "dec";

	case And: return "and";
	case Or:  return "or";
	case Xor: return "xor";

	case Cmp:  return "cmp";
	case Test: return "test";

	case SetL:  return "setl";
	case SetLe: return "setle";
	case SetG:  return "setg";
	case SetGe: return "setge";
	case SetE:  return "sete";
	case SetNE: return "setne";
	case SetZ:  return "setz";
	case SetNZ: return "setnz";

	case Jmp: return "jmp";
	case Jl:  return "jl";
	case Jle: return "jle";
	case Jg:  return "jg";
	case Jge: return "jge";
	case Je:  return "je";
	case Jne: return "jne";
	case Jnz: return "jnz";
	case Jz:  return "jz";

	case Ret: return "ret";
	case Nop: return "nop";
	}

	return "???";
}


// MC -> NASM
std::string X64::MC::ToString() const {
	std::ostringstream result;

	result << ToString(opcode);

	if (opcode == Opcode::Ret ||
		opcode == Opcode::Nop) {
		return result.str();
	}

	result << ' ';

	switch (opcode) {

	case Opcode::Inc:
	case Opcode::Dec:

	case Opcode::SetL:
	case Opcode::SetLe:
	case Opcode::SetE:
	case Opcode::SetNE:
	case Opcode::SetG:
	case Opcode::SetGe:
	case Opcode::SetZ:
	case Opcode::SetNZ:

	case Opcode::Jmp:
	case Opcode::Jz:
	case Opcode::Jnz:
	case Opcode::Jl:
	case Opcode::Jle:
	case Opcode::Je:
	case Opcode::Jne:
	case Opcode::Jg:
	case Opcode::Jge:

		result << ToString(operands[0]);
		break;

	default:

		// NASM uses:
		//
		//     instruction destination, source
		//
		result << ToString(operands[0])
			<< ", "
			<< ToString(operands[1]);

		break;
	}

	return result.str();
}