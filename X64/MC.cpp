#include "pch.h"
#include "MC.hpp"

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
	return { Opcode::SetNE,  dst };
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
	return { Opcode::Jle , dst };
}

X64::MC X64::MC::Je(const Operand dst) {
	return { Opcode::Je , dst };
}

X64::MC X64::MC::Jne(const Operand dst) {
	return { Opcode::Jne , dst };
}

X64::MC X64::MC::Jg(const Operand dst) {
	return { Opcode::Jg , dst };
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
	case Opcode::Jl:	return Jge(operands.Destination());
	case Opcode::Jle:	return Jl(operands.Destination());
	case Opcode::Je:	return Jne(operands.Destination());
	case Opcode::Jne:   return Je(operands.Destination());
	case Opcode::Jg:	return Jle(operands.Destination());
	case Opcode::Jge:   return Jl(operands.Destination());
	case Opcode::Jz:	return Jnz(operands.Destination());
	case Opcode::Jnz:   return Jz(operands.Destination());
	}
	return *this;
}
