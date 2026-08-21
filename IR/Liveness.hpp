#pragma once
#include "BasicBlock.hpp"
#include <unordered_set>

namespace IR {
	class CFG {
	public:
		static CFG Build(std::span<BasicBlock*> blocks);

	private:
		struct BasicBlockInfo {
			BasicBlock* block;
			std::vector<BasicBlockInfo*> predecessors;
			std::vector<BasicBlockInfo*> successors;

			std::unordered_set<IR::Value*> use;
			std::unordered_set<IR::Value*> def;
			std::unordered_set<IR::Value*> liveIn;
			std::unordered_set<IR::Value*> liveOut;
		};

		std::vector<BasicBlockInfo> blocks;
	};
}