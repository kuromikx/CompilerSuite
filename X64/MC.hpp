#pragma once
#include <IR/Value.hpp>

#include "Regs.hpp"
#include "Operand.hpp"

namespace X64 {
	class Operands : public std::array<Operand, 2> {
	public:
		constexpr Operand& Destination() { return at(0); }
		constexpr const Operand& Destination() const { return at(0); }

		constexpr Operand& Source() { return at(1); }
		constexpr const Operand& Source() const { return at(1); }

		constexpr Operand& Left() { return at(0); }
		constexpr const Operand& Left() const { return at(0); }

		constexpr Operand& Right() { return at(1); }
		constexpr const Operand& Right() const { return at(1); }
	};

	struct MC {
		enum class Opcode {
			// Storage
			Mov, MovZx, Push, Pop,
			// Maths
			Add, Sub,
			Inc, Dec,
			// Logic
			And, Or, Xor,
			// Comparisons
			Cmp, Test,
			// SetCC
			SetL, SetLe, SetG, SetGe, SetE, SetNE, SetZ, SetNZ,
			// Branching
			Jmp,
			Jl, Jle,
			Jg, Jge,
			Je, Jne,
			Jnz, Jz,
			Ret,
			Nop
		};

		Opcode opcode;
		Operands operands;

		// Math
		constexpr static MC Add(Operand dst, Operand src);
		constexpr static MC Sub(Operand dst, Operand src);
		constexpr static MC Inc(Operand dst);
		constexpr static MC Dec(Operand dst);

		// Logic
		constexpr static MC And(Operand dst, Operand src);
		constexpr static MC Or(Operand dst, Operand src);
		constexpr static MC Xor(Operand dst, Operand src);

		// Comparisons
		constexpr static MC Cmp(Operand left, Operand right);
		constexpr static MC Test(Operand left, Operand right);
		constexpr static MC SetL(Operand dst);
		constexpr static MC SetLe(Operand ds);
		constexpr static MC SetE(Operand dst);
		constexpr static MC SetNe(Operand dst);
		constexpr static MC SetG(Operand dst);
		constexpr static MC SetGe(Operand dst);
		constexpr static MC SetZ(Operand dst);
		constexpr static MC SetNZ(Operand dst);

		// Branching
		constexpr static MC Jmp(const Operand dst);
		constexpr static MC Jz(const Operand dst);
		constexpr static MC Jnz(const Operand dst);
		constexpr static MC Jl(const Operand dst);
		constexpr static MC Jle(const Operand dst);
		constexpr static MC Je(const Operand dst);
		constexpr static MC Jne(const Operand dst);
		constexpr static MC Jg(const Operand dst);
		constexpr static MC Jge(const Operand dst);
	
		// etc
		constexpr static MC Ret();
		constexpr static MC Nop();

		// Helpers
		constexpr bool IsConditionalJump() const noexcept;
		constexpr bool IsSetCC() const noexcept;
		constexpr MC InvertJumpCondition() const noexcept;
	};

}