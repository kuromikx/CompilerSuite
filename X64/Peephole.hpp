#pragma once
#include "Pass.hpp"

namespace X64 {
	class Peephole : public Pass {
	public:
		bool Optimize(Code& code) override;
	};
}