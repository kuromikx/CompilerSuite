#pragma once
#include <IR/Value.hpp>
#include "Regs.hpp"
#include <cstdint>
#include <variant>

namespace X64 {
	struct Empty {};
	
	struct Address {
		std::uint64_t base;
		std::uint64_t index;
		std::uint64_t scale;
		std::uint64_t displacement;
	};

	struct Immediate {
		std::int64_t value;
	};

	struct Operand : std::variant <
		Empty,
		Register,
		Address,
		Immediate
	> {
		constexpr bool IsRegister() const;
		constexpr bool IsMemoryAddress() const;
		constexpr bool IsImmediate() const;
		constexpr bool IsRax() const;

		constexpr const X64::Register& AsRegister() const;
		constexpr const X64::Address& AsMemoryAddress() const;
		constexpr const X64::Immediate& AsImmediate() const;
	};
}