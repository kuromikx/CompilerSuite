#include "pch.h"
#include "Liveness.hpp"

IR::CFG IR::CFG::Build(std::span<BasicBlock*> blocks) {
	CFG cfg;
	std::unordered_map<BasicBlock*, BasicBlockInfo*> blockToInfo;
	
	cfg.blocks.reserve(blocks.size());

	for (auto& block : blocks) {
		auto& info = cfg.blocks.emplace_back(BasicBlockInfo{ .block = block });
		blockToInfo[block] = &info;
	}

	// find successors
	for (std::size_t blockIndex = 0; blockIndex < blocks.size(); ++blockIndex) {
		BasicBlockInfo& info = cfg.blocks.at(blockIndex);
		auto terminator = info.block->GetTerminator();

		if (!IsTerminator(terminator)) {
			// error
			break;
		}

		const auto addSuccessor = [&](BasicBlock* blk) {
			info.successors.push_back(blockToInfo.at(blk));
		};

		if (std::holds_alternative<Jump>(terminator)) {
			const auto& jmp = std::get<Jump>(terminator);
			addSuccessor(jmp.destination);
		}
		else if (std::holds_alternative<Branch>(terminator)) {
			const auto& bra = std::get<Branch>(terminator);
			addSuccessor(bra.ifTrue);
			addSuccessor(bra.ifFalse);
		}
		else if (std::holds_alternative<Return>(terminator)) {
			const auto& ret = std::get<Return>(terminator);
			// not important
		}
	}

	// find incoming values
	for (auto& blockInfo : cfg.blocks) {
		for (const auto& inst : *blockInfo.block) {
			const auto useValue = [&](Value* value) {
				if (!blockInfo.def.contains(value)) {
					blockInfo.use.insert(value);
				}
			};

			const auto defValue = [&](Value* value) {
				blockInfo.def.insert(value);
			};

			std::visit([&](const auto& i) {
				using T = decltype(i);
				if constexpr (std::derived_from<T, Binary<T>>) {
					auto& binary = (Binary<T>)(i);
					useValue(i.left);
					useValue(i.right);
					defValue(i.result);
				}
				else if constexpr (std::derived_from<T, Unary<T>>) {
					auto& unary = (Unary<T>)(i);
					useValue(i.source);
					defValue(i.dest);
				}
				else if constexpr (std::same_as<T, Branch>) {
					auto& bra = std::get<Branch>(i);
					useValue(bra.condition);
				}
			}, inst);
		}
	}

	return cfg;
}
