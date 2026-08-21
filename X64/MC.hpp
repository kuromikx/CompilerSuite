#pragma once
#include <IR/Value.hpp>

#include "Regs.hpp"
#include "Operand.hpp"

#include <array>

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

		// Storage
		static MC Mov(Operand dst, Operand src);
		static MC MovZx(Operand dst, Operand src);

		// Math
		static MC Add(Operand dst, Operand src);
		static MC Sub(Operand dst, Operand src);
		static MC Inc(Operand dst);
		static MC Dec(Operand dst);

		// Logic
		static MC And(Operand dst, Operand src);
		static MC Or(Operand dst, Operand src);
		static MC Xor(Operand dst, Operand src);

		// Comparisons
		static MC Cmp(Operand left, Operand right);
		static MC Test(Operand left, Operand right);
		static MC SetL(Operand dst);
		static MC SetLe(Operand ds);
		static MC SetE(Operand dst);
		static MC SetNe(Operand dst);
		static MC SetG(Operand dst);
		static MC SetGe(Operand dst);
		static MC SetZ(Operand dst);
		static MC SetNZ(Operand dst);

		// Branching
		static MC Jmp(const Operand dst);
		static MC Jz(const Operand dst);
		static MC Jnz(const Operand dst);
		static MC Jl(const Operand dst);
		static MC Jle(const Operand dst);
		static MC Je(const Operand dst);
		static MC Jne(const Operand dst);
		static MC Jg(const Operand dst);
		static MC Jge(const Operand dst);
	
		// etc
		static MC Ret();
		static MC Nop();

		// Helpers
		bool IsConditionalJump() const noexcept;
		bool IsSetCC() const noexcept;
		MC InvertJumpCondition() const noexcept;
	};

}