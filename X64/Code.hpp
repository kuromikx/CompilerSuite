#pragma once
#include "MCBlock.hpp"

namespace X64 {
	class Code {
	public:
		using Block = MCBlock;

		void CreateBlock(const IR::BasicBlock* irBlock, Label label);
		MCBlock& GetBlock(const IR::BasicBlock* irBlock);
		MCBlock& GetBlock(Label label);
		Label GetLabel(const IR::BasicBlock* irBlock) const;

		auto begin() { return blocks.begin(); }
		auto end() { return blocks.end(); }
		auto cbegin() const { return blocks.cbegin(); }
		auto cend() const { return blocks.cend(); }

	private:
		std::unordered_map<Label, std::unique_ptr<Block>> blocks;
		std::unordered_map<const IR::BasicBlock*, Block*> irBlockToMcBlock;
		std::unordered_map<const IR::BasicBlock*, Label> irBlockToLabel;
	};
}