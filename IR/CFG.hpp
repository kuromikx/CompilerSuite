#pragma once
#include "BasicBlock.hpp"
#include <unordered_set>

namespace IR {
	class CFG {
	public:
		static CFG Build(const Function& function);
		const auto& Blocks() const {
			return blocks;
		}

	private:
		void ComputeLiveness();

		struct BasicBlockInfo {
			const BasicBlock* block;
			std::vector<const BasicBlockInfo*> predecessors;
			std::vector<const BasicBlockInfo*> successors;

			std::unordered_set<IR::Value*> use;
			std::unordered_set<IR::Value*> def;
			std::unordered_set<IR::Value*> liveIn;
			std::unordered_set<IR::Value*> liveOut;
		};
		
		std::vector<BasicBlockInfo> blocks;
	};
}