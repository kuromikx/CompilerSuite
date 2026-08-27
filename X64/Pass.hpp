#pragma once
#include "Code.hpp"

namespace X64 {
	class Pass {
	public:
		virtual ~Pass() = default;
		virtual bool Optimize(Code& code) = 0;
	};
}