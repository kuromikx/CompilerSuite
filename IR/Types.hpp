#pragma once
#include <variant>
#include <memory>
#include <unordered_map>
#include <string>

namespace IR {
	struct FixedWidth;
	struct Pointer;
	struct Array;

	using Type = std::variant<FixedWidth, Pointer, Array>;
	struct FixedWidth { int width; };
	struct Pointer { Type* type; };
	struct Array { Type* type; int length; };

	class Types {
	public:
		Types();
		Type* Int1() const;
		Type* Int8() const;
		Type* Int16() const;
		Type* Int32() const;
		Type* Int64() const;
		Type* GetType(const std::string& name) const;
		Type* AddType(const std::string& name, Type type);

	private:
		std::unordered_map<std::string, std::unique_ptr<Type>> types;
	};
}