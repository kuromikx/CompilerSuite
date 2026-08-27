#include "pch.h"
#include "Code.hpp"

void X64::Code::CreateBlock(const IR::BasicBlock* irBlock, Label label) {
	if (blocks.contains(label)) return;
	if (irBlockToMcBlock.contains(irBlock)) return;

	blocks[label] = std::make_unique<Block>();
	irBlockToMcBlock[irBlock] = blocks.at(label).get();
	irBlockToLabel[irBlock] = label;
}

X64::MCBlock& X64::Code::GetBlock(const IR::BasicBlock* irBlock) {
	return *irBlockToMcBlock.at(irBlock);
}

X64::MCBlock& X64::Code::GetBlock(Label label) {
	return *blocks.at(label);
}

X64::Label X64::Code::GetLabel(const IR::BasicBlock* irBlock) const {
	return irBlockToLabel.at(irBlock);
}
