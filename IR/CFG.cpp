#include "pch.h"
#include "CFG.hpp"

IR::CFG IR::CFG::Build(const Function& function) {
	const auto blocks = function.GetBlocks();

	CFG cfg;
	cfg.blocks.reserve(blocks.size());

	std::unordered_map<const BasicBlock*, BasicBlockInfo*> blockToInfo;

	// Create BasicBlockInfo objects and map each BasicBlock to its info.
	for (const auto& block : blocks) {
		auto& info = cfg.blocks.emplace_back(
			BasicBlockInfo{ .block = block.get() }
		);

		blockToInfo.emplace(block.get(), &info);
	}

	// Find CFG successors/predecessors.
	for (std::size_t blockIndex = 0; blockIndex < blocks.size(); ++blockIndex) {
		auto& info = cfg.blocks.at(blockIndex);
		auto terminator = info.block->GetTerminator();

		if (!IsTerminator(terminator)) {
			continue;
		}

		const auto addSuccessor = [&](BasicBlock* block) {
			auto* successor = blockToInfo.at(block);

			info.successors.push_back(successor);
			successor->predecessors.push_back(&info);
		};

		std::visit([&](const auto& instruction) {
			using T = std::decay_t<decltype(instruction)>;

			if constexpr (std::same_as<T, Jump>) {
				addSuccessor(instruction.destination);
			}
			else if constexpr (std::same_as<T, Branch>) {
				addSuccessor(instruction.ifTrue);
				addSuccessor(instruction.ifFalse);
			}
			else if constexpr (std::same_as<T, Return>) {
				// Return has no successors.
			}
		}, terminator);
	}

	// Find block-local uses and definitions.
	for (auto& blockInfo : cfg.blocks) {
		const auto useValue = [&](Value* value) {
			if (value != nullptr && !blockInfo.def.contains(value)) {
				blockInfo.use.insert(value);
			}
		};

		const auto defValue = [&](Value* value) {
			if (value != nullptr) {
				blockInfo.def.insert(value);
			}
		};

		for (const auto& inst : *blockInfo.block) {
			std::visit([&](const auto& instruction) {
				using T = std::decay_t<decltype(instruction)>;

				if constexpr (std::derived_from<T, Binary<T>>) {
					useValue(instruction.left);
					useValue(instruction.right);
					defValue(instruction.result);
				}
				else if constexpr (std::derived_from<T, Unary<T>>) {
					useValue(instruction.source);
					defValue(instruction.destination);
				}
				else if constexpr (std::same_as<T, Branch>) {
					useValue(instruction.condition);
				}
				else if constexpr (std::same_as<T, Return>) {
					useValue(instruction.value);
				}
				}, inst);
		}
	}

	cfg.ComputeLiveness();

	return cfg;
}

void IR::CFG::ComputeLiveness() {
	bool changed;

	do {
		changed = false;

		for (auto blockIt = blocks.rbegin(); blockIt != blocks.rend(); ++blockIt) {
			auto& block = *blockIt;

			std::unordered_set<Value*> newLiveOut;
			
			for (const auto& successor : block.successors) {
				newLiveOut.insert(
					successor->liveIn.begin(),
					successor->liveIn.end()
				);
			}

			std::unordered_set<Value*> newLiveIn = block.use;

			for (auto* value : newLiveOut) {
				if (!block.def.contains(value)) {
					newLiveIn.insert(value);
				}
			}

			if (newLiveOut != block.liveOut || 
				newLiveIn != block.liveIn
			) {
				changed = true;

				block.liveIn = std::move(newLiveIn);
				block.liveOut = std::move(newLiveOut);
			}
		}

	} while (changed);
}
