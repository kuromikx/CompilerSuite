#include "pch.h"
#include "InterferenceGraph.hpp"

IR::InterferenceGraph IR::InterferenceGraph::Build(const CFG& cfg) {
	InterferenceGraph graph;

	for (const auto& blockInfo : cfg.Blocks()) {
		auto live = blockInfo.liveOut;
		const auto& block = blockInfo.block;

		for (auto it = block->rbegin(); it != block->rend(); ++it) {
			std::visit([&](const auto& instruction) {
				ForEachDefinition(instruction, [&](Value* definition) {
					graph.AddValue(definition);

					for (auto* liveValue : live) {
						graph.AddInterference(definition, liveValue);
					}

					live.erase(definition);
					});

				ForEachUse(instruction, [&](Value* use) {
					if (use != nullptr) {
						graph.AddValue(use);
						live.insert(use);
					}
				});
			}, *it);
		}
	}


	return graph;
}

void IR::InterferenceGraph::AddValue(Value* value) {
	if (value != nullptr) {
		edges.try_emplace(value);
	}
}

void IR::InterferenceGraph::AddInterference(Value* a, Value* b) {
	if (a == nullptr || b == nullptr || a == b)
		return;

	AddValue(a);
	AddValue(b);

	edges.at(a).insert(b);
	edges.at(b).insert(a);
}