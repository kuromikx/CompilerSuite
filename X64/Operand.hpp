#pragma once
#include <IR/BasicBlock.hpp>
#include "Regs.hpp"
#include <cstdint>
#include <variant>

namespace X64 {
	struct Empty {};
	
	struct Address {
		Register base;
		std::int64_t offset;
		std::int64_t index;
		std::int64_t scale;
		bool operator == (const Address&) const = default;
	};

	struct Immediate {
		std::int64_t value;
		bool operator == (const Immediate&) const = default;
	};

	struct Label {
		std::variant<std::string, std::size_t> id;
		bool operator == (const Label&) const = default;
		std::string ToString() const;
	};

	struct Operand : std::variant <
		Register,
		Address,
		Immediate,
		Label
	> {
		bool IsRegister() const;
		bool IsMemoryAddress() const;
		bool IsImmediate() const;
		bool IsRax() const;
		bool IsLabel() const;

		const X64::Register& AsRegister() const;
		const X64::Address& AsMemoryAddress() const;
		const X64::Immediate& AsImmediate() const;
		const X64::Label& AsLabel() const;
		bool operator == (const Operand& rhs) const;
	};
}

namespace std {
	template<>
	struct hash<X64::Label> {
		std::size_t operator()(const X64::Label& label) const noexcept {
			if (std::holds_alternative<std::size_t>(label.id)) {
				return std::hash<std::size_t>{}(std::get<std::size_t>(label.id));
			}
			if (std::holds_alternative<std::string>(label.id)) {
				return std::hash<std::string>{}(std::get<std::string>(label.id));
			}
			throw;
		}
	};
}