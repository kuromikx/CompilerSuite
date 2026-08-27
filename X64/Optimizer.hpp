#pragma once
#include "Pass.hpp"

namespace X64 {
	class Optimizer {
	public:
		enum Flags {
		};
		
		template<typename Pass>
		void Add() {
			passes.emplace_back(
				std::make_unique<Pass>()
			);
		}

		void Optimize(Code& code);

	public:
		std::vector<std::unique_ptr<Pass>> passes;
	};
}