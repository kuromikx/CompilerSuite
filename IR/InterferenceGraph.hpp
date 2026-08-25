#pragma once
#include "CFG.hpp"
#include <unordered_map>
#include <unordered_set>

namespace IR {
	class InterferenceGraph {
	public:
		static InterferenceGraph Build(const CFG& cfg);
	
		const auto& GetNeighbours(Value* value) const {
			return edges.at(value);
		}

	private:
		void AddValue(Value* value);
		void AddInterference(Value* a, Value* b);
	
		std::unordered_map<Value*, std::unordered_set<Value*>> edges;
	};
}