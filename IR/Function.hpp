#pragma once
#include <memory>
#include "BasicBlock.hpp"

namespace IR {
	class Function {
	public:
		BasicBlock* CreateBlock();
		Value* CreateVariable(Type* type, VariableMetadata metadata = {.name = "unnamed"});
		Value* CreateConstant(Type* type, Constant::Literal literal);
		Value* CreateTemporary(Type* type);

	protected:
		template<typename Ty, typename... Args>
		Value* CreateValue(Args&&... args) {
			values.emplace_back(new Ty(std::forward<Args>(args)...));
			return values.back().get();
		}

		std::vector<std::unique_ptr<BasicBlock>> blocks;
		std::vector<std::unique_ptr<Value>> values;
	};
}